#pragma once
#include <optional>

#include "absl/status/statusor.h"
#include "ballot.h"
#include "store.h"

using absl::StatusOr;

class PrepareRequest {
  public:
    PrepareRequest(Ballot ballot, const Key& key)
        : ballot_(ballot), key_(key) {}
    inline Ballot Ballot() const { return ballot_; }
    inline Key Key() const { return key_; }

  private:
    class ::Ballot ballot_;
    class ::Key key_;
};

class PrepareResponse {
  public:
    inline std::optional<Ballot> Ballot() const { return ballot_; }
    inline std::optional<Value> Value() const { return value_; }
    inline uint64_t NodeId() const { return node_id_; }
    inline bool Ok() const { return promise_ok; }

    static inline PrepareResponse PrepareOk(uint64_t node_id) {
        return PrepareResponse(node_id);
    }

    static inline PrepareResponse PrepareError(
        uint64_t node_id, class ::Ballot ballot,
        std::optional<class ::Value> value = std::nullopt) {
        return PrepareResponse(node_id, ballot, value);
    }

  private:
    PrepareResponse(uint64_t node_id) : node_id_(node_id), promise_ok(true) {}
    PrepareResponse(uint64_t node_id, class ::Ballot ballot,
                    std::optional<class ::Value> value)
        : node_id_(node_id),
          promise_ok(false),
          ballot_(ballot),
          value_(value) {}
    std::optional<class ::Ballot> ballot_;
    std::optional<class ::Value> value_;
    uint64_t node_id_;
    bool promise_ok;
};

class AcceptRequest {
  public:
    AcceptRequest(Ballot ballot, Key key, Value value)
        : ballot_(ballot), key_(key), value_(value) {}
    inline Ballot Ballot() const { return ballot_; }
    inline Key Key() const { return key_; }
    inline Value Value() const { return value_; }

  private:
    class ::Ballot ballot_;
    class ::Key key_;
    class ::Value value_;
};

class AcceptResponse {
  public:
    inline std::optional<Ballot> Ballot() const { return ballot_; }
    inline std::optional<Value> Value() const { return value_; }
    inline uint64_t NodeId() const { return node_id_; }
    inline bool Ok() const { return accept_ok; }

    static inline AcceptResponse AcceptOk(uint64_t node_id) {
        return AcceptResponse(node_id);
    }

    static inline AcceptResponse AcceptError(
        uint64_t node_id, class ::Ballot ballot,
        std::optional<class ::Value> value = std::nullopt) {
        return AcceptResponse(node_id, ballot, value);
    }

  private:
    AcceptResponse(uint64_t node_id) : node_id_(node_id), accept_ok(true) {}
    AcceptResponse(uint64_t node_id, class ::Ballot ballot,
                   std::optional<class ::Value> value)
        : node_id_(node_id), ballot_(ballot), value_(value), accept_ok(false) {}
    std::optional<class ::Ballot> ballot_;
    std::optional<class ::Value> value_;
    uint64_t node_id_;
    bool accept_ok;
};

class Transport {
  public:
    virtual StatusOr<PrepareResponse> SendPrepare(PrepareRequest) = 0;
    virtual StatusOr<AcceptResponse> SendAccept(AcceptRequest) = 0;
    virtual ~Transport() {};
};
