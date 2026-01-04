#include <map>
#include <mutex>

#include "store.h"

class InMemoryStore : public Store {
  public:
    void Set(const Key&, const Value&) override;
    Value Get(const Key&) override;
    bool Has(const Key&) override;
    void Delete(const Key&) override;

  private:
    std::map<std::vector<std::byte>, std::vector<std::byte>> store_;
    std::mutex mutex_;
};
