#pragma once
#include <optional>

#include "ballot.h"
#include "store.h"

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
    PrepareResponse(uint64_t node_id, bool prepared)
        : node_id_(node_id),
          promised_ballot_(node_id),
          accepted_ballot_(node_id),
          prepared_(prepared) {}
    inline Ballot PromisedBallot() const { return promised_ballot_; }
    inline Ballot AcceptedBallot() const { return accepted_ballot_; }
    inline std::optional<Value> Value() const { return value_; }
    inline uint64_t NodeId() const { return node_id_; }
    inline bool Prepared() const { return prepared_; }

  private:
    Ballot promised_ballot_;
    Ballot accepted_ballot_;
    std::optional<class ::Value> value_;
    uint64_t node_id_;
    bool prepared_;
};

class AcceptRequest {
  public:
    AcceptRequest(Ballot ballot, Key key, Value value_new)
        : ballot_(ballot), key_(key), value_new_(value_new) {}
    inline Ballot Ballot() const { return ballot_; }
    inline Key Key() const { return key_; }
    inline Value ValueNew() const { return value_new_; }

  private:
    class ::Ballot ballot_;
    class ::Key key_;
    Value value_new_;
};

class AcceptResponse {
  public:
    AcceptResponse(uint64_t node_id)
        : node_id_(node_id),
          promised_ballot_(node_id),
          accepted_ballot_(node_id) {}
    inline Ballot PromisedBallot() const { return promised_ballot_; }
    inline Ballot AcceptedBallot() const { return accepted_ballot_; }
    inline std::optional<Value> Value() const { return value_; }
    inline uint64_t NodeId() const { return node_id_; }

  private:
    Ballot promised_ballot_;
    Ballot accepted_ballot_;
    std::optional<class ::Value> value_;
    uint64_t node_id_;
};

class Transport {
  public:
    virtual PrepareResponse SendPrepare(PrepareRequest) = 0;
    virtual AcceptResponse SendAccept(AcceptRequest) = 0;
    virtual ~Transport() {};
};
