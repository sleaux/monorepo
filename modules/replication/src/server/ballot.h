#pragma once
#include <cstdint>

#include "absl/strings/str_format.h"
class Ballot {
  public:
    Ballot(uint64_t node_id) : node_id_(node_id) { counter_ = 0; };
    inline uint64_t Increment() {
        counter_++;
        return counter_;
    }
    inline uint64_t FastForwardAfter(uint64_t counter) {
        counter_ = counter + 1;
        return counter_;
    }
    inline uint64_t Counter() { return counter_; }
    inline uint64_t NodeId() { return node_id_; }
    inline std::string AsString() {
        return absl::StrFormat("Ballot(node_id_: %d, counter_: %d", node_id_,
                               counter_);
    }

  private:
    uint64_t counter_;
    uint64_t node_id_;
};
