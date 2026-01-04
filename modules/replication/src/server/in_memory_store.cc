#include "in_memory_store.h"

#include <mutex>

Value InMemoryStore::Get(const Key& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return Value(store_.at(key.AsByte()));
}

void InMemoryStore::Set(const Key& key, const Value& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_[key.AsByte()] = value.AsByte();
}

bool InMemoryStore::Has(const Key& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.find(key.AsByte()) != store_.end();
}
