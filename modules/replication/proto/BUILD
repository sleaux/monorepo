load("@rules_proto_grpc_cpp//:defs.bzl", "cpp_grpc_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "caspaxos",
    srcs = ["caspaxos.proto"],
)

cpp_grpc_library(
    name = "caspaxos_cc",
    protos = [":caspaxos"],
)
