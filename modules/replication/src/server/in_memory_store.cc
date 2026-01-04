#include "in_memory_store.h"

#include <mutex>

Value InMemoryStore::Get(const Key& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return Value(store_.at(key.AsBytes()));
}

void InMemoryStore::Set(const Key& key, const Value& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_[key.AsBytes()] = value.AsBytes();
}

bool InMemoryStore::Has(const Key& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.find(key.AsBytes()) != store_.end();
}

void InMemoryStore::Delete(const Key& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_.erase(key.AsBytes());
}
