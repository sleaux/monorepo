#pragma once
#include <cstddef>
#include <optional>
#include <vector>

class Key {
  public:
    Key(const std::vector<std::byte> value) : value_(value) {}
    inline std::vector<std::byte> AsBytes() const { return value_; }

  private:
    std::vector<std::byte> value_;
};

class Value {
  public:
    Value(const std::vector<std::byte> value) : value_(value) {}
    Value() {};
    inline std::vector<std::byte> AsBytes() const { return value_.value(); }
    inline bool Exists() const { return value_.has_value(); }

  private:
    std::optional<std::vector<std::byte>> value_;
};

class Store {
  public:
    // TODO, improve interface to support failure reading/writing to the store
    virtual void Set(const Key& key, const Value& value) = 0;
    virtual void Delete(const Key& key) = 0;
    virtual Value Get(const Key& key) = 0;
    virtual bool Has(const Key& key) = 0;
    virtual ~Store() {};
};
