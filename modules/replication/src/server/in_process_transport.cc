#include "in_process_transport.h"

#include "transport.h"

InProcessTransport::InProcessTransport(std::shared_ptr<Node> node)
    : node_(node) {}

StatusOr<PrepareResponse> InProcessTransport::SendPrepare(
    PrepareRequest request) {
    return node_->Prepare(request);
}

StatusOr<AcceptResponse> InProcessTransport::SendAccept(AcceptRequest request) {
    return node_->Accept(request);
}
