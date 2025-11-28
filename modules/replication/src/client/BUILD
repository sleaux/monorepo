cc_binary(
    name = "client",
    srcs = ["main.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//modules/replication/proto:foo_cc_proto",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/flags:usage",
    ],
)
