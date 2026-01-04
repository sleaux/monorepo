#include "node.h"

#include <memory>
#include <mutex>

#include "modules/replication/src/server/transport.h"

Node::Node(uint64_t id, std::unique_ptr<Store> store)
    : id_(id), store_(std::move(store)), ballot_(id) {}

void Node::AddTransport(std::unique_ptr<class ::Transport> transport) {
    transport = std::move(transport);
}

PrepareResponse Node::Prepare(PrepareRequest request) {
    // TODO Improve to be lock per key
    std::lock_guard<std::mutex> lock(mutex_);

    auto key = request.Key();
    auto ballot_accepted_key = BallotAcceptedKey(key);
    auto ballot_promised_key = BallotPromisedKey(key);
    Value value;
    Value ballot_accepted_value;
    Value ballot_promised_value;

    if (store_->Has(key)) {
        value = store_->Get(key);
    }

    if (store_->Has(ballot_accepted_key)) {
        ballot_accepted_value = store_->Get(ballot_accepted_key);
    }

    if (store_->Has(ballot_promised_key)) {
        ballot_promised_value = store_->Get(ballot_promised_key);
    }

    return PrepareResponse(id_, true);
}

AcceptResponse Node::Accept(AcceptRequest request) {
    // TODO
    return AcceptResponse(id_);
}

Value Node::Change(const Key& key, const ChangeFunction& f) {
    auto value_current = SendPrepare(key);

    return SendAccept(key, value_current, f);
}

Value Node::SendPrepare(const Key& key) {
    // TODO
    auto acceptors_count = nodes_.size();
    auto confirmations_needed = (acceptors_count - 1) / 2 + 1;

    ballot_.Increment();

    return Value(std::byte{0});
}

Value Node::SendAccept(const Key& key, const Value& state_current,
                       const ChangeFunction& f) {
    // TODO
    return Value(std::byte{0});
}
