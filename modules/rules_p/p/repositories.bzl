VERSIONS = {
    "3.0.0": {
        "url": "https://github.com/p-org/P/archive/refs/tags/p-3.0.0.tar.gz",
        "sha256": "ZDc4MDRjZTA5N2Q2OTE2YzliN2E2NGRkMmVhMGJkNzI2YjI3MDg1ZmU4ODUxYTI4MjM1NTkzODQ5Yjk4Yjk2ZSAgUC1wLTMuMC4wcLnRhci5nego=",
    },
}

PLATFORMS = {
    "x86_64-apple-darwin": struct(
        compatible_with = [
            "@platforms//os:macos",
            "@platforms//cpu:x86_64",
        ],
    ),
    "aarch64-apple-darwin": struct(
        compatible_with = [
            "@platforms//os:macos",
            "@platforms//cpu:aarch64",
        ],
    ),
    "x86_64-unknown-linux-gnu": struct(
        compatible_with = [
            "@platforms//os:linux",
            "@platforms//cpu:x86_64",
        ],
    ),
    "arm64-unknown-linux-gnu": struct(
        compatible_with = [
            "@platforms//os:linux",
            "@platforms//cpu:arm64",
        ],
    ),
    "x86_64-pc-windows-msvc": struct(
        compatible_with = [
            "@platforms//os:windows",
            "@platforms//cpu:x86_64",
        ],
    ),
    "arm64-pc-windows-msvc": struct(
        compatible_with = [
            "@platforms//os:windows",
            "@platforms//cpu:arm64",
        ],
    ),
}

def _toolchains_repo(repository_ctx):
    build_content = ""

    for [platform, meta] in PLATFORMS.items():
        build_content += """
toolchain(
    name = "{platform}_toolchain",
    exec_compatible_with = {compatible_with},
    toolchain = "@{repository_name}_{platform}//:p_toolchain",
    toolchain_type = "@rules_p//p:toolchain_type",
)
""".format(
            platform = platform,
            name = repository_ctx.attr.name,
            repository_name = repository_ctx.attr.repository_name,
            compatible_with = meta.compatible_with,
        )

    repository_ctx.file("BUILD.bazel", build_content)

toolchains_repo = repository_rule(
    implementation = _toolchains_repo,
    doc = "",
    attrs = {
        "repository_name": attr.string(),
    },
)

def _p_repository(repository_ctx):
    url = VERSIONS[repository_ctx.attr.p_version]["url"]
    sha256 = VERSIONS[repository_ctx.attr.p_version]["sha256"]
    repository_ctx.download_and_extract(
        url = url,
        # sha256 = sha256
    )

    build_content = """
load("@rules_p//p:toolchain.bzl", "p_toolchain")
load(
    "@rules_dotnet//dotnet:defs.bzl",
    "csharp_binary",
    "csharp_library",
    "publish_binary",
)

csharp_library(
    name = "PCheckerCore",
    srcs = glob(["**/Src/PChecker/CheckerCore/**/*.cs"]),
    target_frameworks = ["net8.0"],
)

csharp_library(
    name = "CompilerCore",
    srcs = glob(["**/Src/PCompiler/CompilerCore/**/*.cs"]) + ["@rules_p//p/dependencies/{p_version}/parser:parser"],
    target_frameworks = ["net8.0"],
    deps = [
        "@paket.deps//antlr4",
        "@paket.deps//antlr4.runtime",
        "@paket.deps//languageext.core",
        "@paket.deps//litedb",
        "@paket.deps//microsoft.netcore.targets",
        "@paket.deps//system.runtime.loader",
        "@paket.deps//system.valuetuple",
    ],
)

csharp_binary(
    name = "p_command_line",
    srcs = glob(["**/Src/PCompiler/PCommandLine/**/*.cs"]),
    out = "p",
    target_frameworks = ["net8.0"],
    winexe = True,
    deps = [
        ":CompilerCore",
        ":PCheckerCore",
    ],
)

publish_binary(
    name = "p",
    binary = ":p_command_line",
    self_contained = True,
    target_framework = "net8.0",
    visibility = ["//visibility:public"],
)

p_toolchain(
    name = "p_toolchain",
    p = ":p",
)
    """.format(p_version = repository_ctx.attr.p_version)
    repository_ctx.file("BUILD.bazel", build_content)

p_repository = repository_rule(
    implementation = _p_repository,
    doc = "",
    attrs = {
        "p_version": attr.string(mandatory = True, values = VERSIONS.keys()),
        "platform": attr.string(mandatory = True, values = PLATFORMS.keys()),
    },
)

def p_register_toolchains(name, p_version, register = True, **kwargs):
    for platform in PLATFORMS.keys():
        p_repository(
            name = name + "_" + platform,
            platform = platform,
            p_version = p_version,
            **kwargs
        )

        if register:
            native.register_toolchain("@{}_toolchains//:{}_toolchain".format(name, platform))

    toolchains_repo(
        name = name + "_toolchains",
        repository_name = name,
    )
