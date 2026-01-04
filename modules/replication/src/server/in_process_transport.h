#pragma once
#include "node.h"
#include "transport.h"

class InProcessTransport : public Transport {
  public:
    InProcessTransport(std::shared_ptr<Node>);
    StatusOr<PrepareResponse> SendPrepare(PrepareRequest) override;
    StatusOr<AcceptResponse> SendAccept(AcceptRequest) override;

  private:
    std::shared_ptr<Node> node_;
};
