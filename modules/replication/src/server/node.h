#pragma once

#include <memory>
#include <mutex>
#include <set>

#include "absl/status/statusor.h"
#include "ballot.h"
#include "change_function.h"
#include "store.h"
#include "transport.h"

using absl::StatusOr;
class Node;

class Node {
  public:
    Node(uint64_t, std::unique_ptr<Store>);
    void AddTransport(std::unique_ptr<Transport>);

    template <typename... Nodes>
    inline void AddNodes(Nodes... nodes) {
        nodes_ = {nodes...};
    }
    StatusOr<PrepareResponse> Prepare(PrepareRequest);
    StatusOr<AcceptResponse> Accept(AcceptRequest);
    StatusOr<Value> Change(const Key&, const ChangeFunction&);
    inline std::shared_ptr<Transport> Transport() { return transport_; }

  private:
    StatusOr<Value> SendPrepare(const Key&);
    StatusOr<Value> SendAccept(const Key&, const Value&);

    inline Key BallotAcceptedKey(const Key& key) {
        // TODO, strategy to reserve keys
        auto buffer = key.AsBytes();
        buffer.push_back(std::byte{0});
        return Key(buffer);
    }

    inline Key BallotPromisedKey(const Key& key) {
        // TODO, strategy to reserve keys
        auto buffer = key.AsBytes();
        buffer.push_back(std::byte{1});
        return Key(buffer);
    }

    uint64_t id_;
    Ballot ballot_;
    std::set<std::shared_ptr<Node>> nodes_;
    std::mutex mutex_;
    std::unique_ptr<Store> store_;
    std::shared_ptr<class ::Transport> transport_;
};

template <typename... Nodes>
inline void MingleNodes(Nodes... nodes) {
    (std::forward<Nodes>(nodes)->AddNodes(nodes), ...);
}
