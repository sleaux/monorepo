#pragma once
#include <cstdint>
#include <vector>

#include "absl/strings/str_format.h"
#include "store.h"
class Ballot {
  public:
    Ballot(uint64_t node_id, uint64_t counter = 0) : node_id_(node_id) {
        counter_ = counter;
    };
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
    inline Value AsValue() const {
        std::vector<std::byte> buffer(16);

        std::memcpy(buffer.data(), &counter_, sizeof counter_);
        std::memcpy(&buffer.data()[8], &node_id_, sizeof node_id_);
        return Value(buffer);
    }

    static Ballot FromValue(const Value& value) {
        uint64_t node_id;
        uint64_t counter;
        std::memcpy(&counter, value.AsBytes().data(), sizeof counter);
        std::memcpy(&node_id, value.AsBytes().data(), sizeof node_id);
        return Ballot(node_id, counter);
    }

  private:
    uint64_t counter_;
    uint64_t node_id_;
};
