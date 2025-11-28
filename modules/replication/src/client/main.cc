#include <iostream>
#include <optional>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "grpc++/grpc++.h"
#include "modules/replication/proto/foo.grpc.pb.h"

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");
ABSL_FLAG(std::optional<int32_t>, value, std::nullopt, "Value to set");

using foo::Foo;
using foo::GetReply;
using foo::GetRequest;
using foo::SetReply;
using foo::SetRequest;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class FooClient {
   public:
    FooClient(std::shared_ptr<Channel> channel) : stub(Foo::NewStub(channel)) {}

    void Set(const int32_t value) {
        SetRequest request;
        request.set_value(value);
        SetReply reply;
        ClientContext context;
        Status status = stub->Set(&context, request, &reply);
        if (!status.ok()) {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
        }
    }

    int32_t Get() {
        GetRequest request;
        GetReply reply;
        ClientContext context;
        Status status = stub->Get(&context, request, &reply);
        if (status.ok()) {
            return reply.value();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return -1;
        }
    }

   private:
    std::unique_ptr<Foo::Stub> stub;
};

int main(int argc, char* argv[]) {
    absl::ParseCommandLine(argc, argv);
    std::string target_str = absl::GetFlag(FLAGS_target);

    FooClient client(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    if (absl::GetFlag(FLAGS_value).has_value()) {
        client.Set(absl::GetFlag(FLAGS_value).value());
    }

    std::cout << target_str << " returned: " << client.Get() << std::endl;

    return 0;
}
