#include <iostream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "grpc++/grpc++.h"
#include "modules/replication/proto/foo.grpc.pb.h"

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

int32_t state;

using foo::Foo;
using foo::GetReply;
using foo::GetRequest;
using foo::SetReply;
using foo::SetRequest;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class FooServiceImpl final : public Foo::Service {
    Status Get(ServerContext* context, const GetRequest* request,
               GetReply* reply) override {
        reply->set_value(state);
        return Status::OK;
    }
    Status Set(ServerContext* context, const SetRequest* request,
               SetReply* reply) override {
        state = request->value();
        reply->set_successful(true);
        return Status::OK;
    }
};

void RunServer(uint16_t port) {
    std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
    FooServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char* argv[]) {
    absl::ParseCommandLine(argc, argv);
    RunServer(absl::GetFlag(FLAGS_port));
    return 0;
}
