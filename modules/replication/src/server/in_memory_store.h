#include <map>
#include <mutex>

#include "store.h"

class InMemoryStore : public Store {
  public:
    void Set(const Key& key, const Value& value) override;
    Value Get(const Key& key) override;
    bool Has(const Key& key) override;

  private:
    std::map<std::byte, std::byte> store_;
    std::mutex mutex_;
};
