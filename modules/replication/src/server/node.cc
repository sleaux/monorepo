#include "node.h"

#include <memory>
#include <mutex>

#include "modules/replication/src/server/transport.h"

Node::Node(uint64_t id, std::unique_ptr<Store> store)
    : id_(id), store_(std::move(store)), ballot_(id) {}

void Node::AddTransport(std::unique_ptr<class ::Transport> transport) {
    transport = std::move(transport);
}

StatusOr<PrepareResponse> Node::Prepare(PrepareRequest request) {
    // TODO Improve to be lock per key
    std::lock_guard<std::mutex> lock(mutex_);

    auto key = request.Key();
    auto ballot = request.Ballot();
    auto ballot_accepted_key = BallotAcceptedKey(key);
    auto ballot_promised_key = BallotPromisedKey(key);
    Value ballot_accepted_value;
    Value ballot_promised_value;

    std::optional<Value> value = std::nullopt;
    if (store_->Has(key)) {
        value = store_->Get(key);
    }

    if (store_->Has(ballot_accepted_key)) {
        ballot_accepted_value = store_->Get(ballot_accepted_key);
        Ballot ballot_accepted =
            Ballot::FromValue(ballot_accepted_value.AsBytes());
        if (ballot_accepted.Counter() > ballot.Counter()) {
            return PrepareResponse::PrepareError(id_, ballot_accepted, value);
        } else if (ballot_accepted.Counter() == ballot.Counter() &&
                   ballot_accepted.NodeId() > ballot.NodeId()) {
            return PrepareResponse::PrepareError(id_, ballot_accepted, value);
        }
    }

    if (store_->Has(ballot_promised_key)) {
        ballot_promised_value = store_->Get(ballot_promised_key);
        Ballot ballot_promised =
            Ballot::FromValue(ballot_accepted_value.AsBytes());
        if (ballot_promised.Counter() > ballot.Counter()) {
            return PrepareResponse::PrepareError(id_, ballot_promised, value);
        } else if (ballot_promised.Counter() == ballot.Counter() &&
                   ballot_promised.NodeId() > ballot.NodeId()) {
            return PrepareResponse::PrepareError(id_, ballot_promised, value);
        }
    }

    store_->Set(ballot_promised_key, ballot.AsValue());

    return PrepareResponse::PrepareOk(id_);
}

StatusOr<AcceptResponse> Node::Accept(AcceptRequest request) {
    // TODO Improve to be lock per key
    std::lock_guard<std::mutex> lock(mutex_);

    auto key = request.Key();
    auto ballot = request.Ballot();
    auto ballot_accepted_key = BallotAcceptedKey(key);
    auto ballot_promised_key = BallotPromisedKey(key);
    Value ballot_accepted_value;
    Value ballot_promised_value;

    std::optional<Value> value = std::nullopt;
    if (store_->Has(key)) {
        value = store_->Get(key);
    }

    if (store_->Has(ballot_accepted_key)) {
        ballot_accepted_value = store_->Get(ballot_accepted_key);
        Ballot ballot_accepted =
            Ballot::FromValue(ballot_accepted_value.AsBytes());
        if (ballot_accepted.Counter() > ballot.Counter()) {
            return AcceptResponse::AcceptError(id_, ballot_accepted, value);
        } else if (ballot_accepted.Counter() == ballot.Counter() &&
                   ballot_accepted.NodeId() > ballot.NodeId()) {
            return AcceptResponse::AcceptError(id_, ballot_accepted, value);
        }
    }

    if (store_->Has(ballot_promised_key)) {
        ballot_promised_value = store_->Get(ballot_promised_key);
        Ballot ballot_promised =
            Ballot::FromValue(ballot_accepted_value.AsBytes());
        if (ballot_promised.Counter() > ballot.Counter()) {
            return AcceptResponse::AcceptError(id_, ballot_promised, value);
        } else if (ballot_promised.Counter() == ballot.Counter() &&
                   ballot_promised.NodeId() > ballot.NodeId()) {
            return AcceptResponse::AcceptError(id_, ballot_promised, value);
        }
    }

    store_->Delete(ballot_promised_key);
    store_->Set(ballot_accepted_key, ballot.AsValue());
    store_->Set(key, request.Value());

    return AcceptResponse::AcceptOk(id_);
}

StatusOr<Value> Node::Change(const Key& key, const ChangeFunction& f) {
    auto result = SendPrepare(key);
    if (!result.ok()) {
        return result;
    }

    auto state_new = f(result.value());

    return SendAccept(key, state_new);
}

StatusOr<Value> Node::SendPrepare(const Key& key) {
    auto acceptors_count = nodes_.size();
    auto confirmations_needed = (acceptors_count - 1) / 2 + 1;
    size_t confirmations = 0;
    uint64_t highest_ballot_seen = 0;
    Value current_state;

    ballot_.Increment();

    PrepareRequest request(ballot_, key);

    for (const auto& node : nodes_) {
        auto result = transport_->SendPrepare(request);
        if (result.ok()) {
            PrepareResponse response = result.value();
            if (response.Ok()) {
                confirmations++;
                current_state = response.Value().value();
            } else {
                highest_ballot_seen =
                    std::max(highest_ballot_seen, response.Ballot()->Counter());
            }
        }
    }

    if (confirmations < confirmations_needed) {
        ballot_.FastForwardAfter(highest_ballot_seen);
        return absl::FailedPreconditionError(
            "Did not get enough confirmations");
    }

    return current_state;
}

StatusOr<Value> Node::SendAccept(const Key& key, const Value& value) {
    auto acceptors_count = nodes_.size();
    auto confirmations_needed = (acceptors_count - 1) / 2 + 1;
    size_t confirmations = 0;
    uint64_t highest_ballot_seen = 0;
    Value current_state;

    ballot_.Increment();

    AcceptRequest request(ballot_, key, value);

    for (const auto& node : nodes_) {
        auto result = transport_->SendAccept(request);
        if (result.ok()) {
            AcceptResponse response = result.value();
            if (response.Ok()) {
                confirmations++;
                current_state = response.Value().value();
            } else {
                highest_ballot_seen =
                    std::max(highest_ballot_seen, response.Ballot()->Counter());
            }
        }
    }

    if (confirmations < confirmations_needed) {
        ballot_.FastForwardAfter(highest_ballot_seen);
        return absl::FailedPreconditionError(
            "Did not get enough confirmations");
    }

    return current_state;
}
