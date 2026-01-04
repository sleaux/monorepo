#pragma once
#include <cstddef>
#include <optional>

class Key {
  public:
    Key(std::byte value) : value_(value) {}
    inline std::byte AsByte() const { return value_; }

  private:
    std::byte value_;
};

class Value {
  public:
    Value(std::byte value) : value_(value) {}
    Value() = default;
    inline bool Exists() const { return value_.has_value(); }
    inline std::byte AsByte() const { return value_.value(); }

  private:
    std::optional<std::byte> value_;
};

class Store {
  public:
    virtual void Set(const Key& key, const Value& value) = 0;
    virtual Value Get(const Key& key) = 0;
    virtual bool Has(const Key& key) = 0;
    virtual ~Store() {};
};
