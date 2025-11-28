load("@rules_proto_grpc_cpp//:defs.bzl", "cpp_grpc_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "foo_proto",
    srcs = ["foo.proto"],
)

cpp_grpc_library(
    name = "foo_cc_proto",
    protos = [":foo_proto"],
)
