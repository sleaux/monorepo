load("//p/private:providers.bzl", "PInfo")

def _p_toolchain(ctx):
    dotnet_toolchain = ctx.toolchains["@rules_dotnet//dotnet:toolchain_type"]

    default = DefaultInfo()
    p_info = PInfo(
        p = ctx.attr.p,
        p_files = [],
        dotnet = dotnet_toolchain.runtime,
        dotnet_files = dotnet_toolchain.dotnetinfo.runtime_files,
    )
    toolchain_info = platform_common.ToolchainInfo(
        default = default,
        p_info = p_info,
    )
    return [
        default,
        p_info,
        toolchain_info,
    ]

p_toolchain = rule(
    doc = "",
    implementation = _p_toolchain,
    attrs = {
        "p": attr.label(
            doc = "",
            executable = True,
            cfg = "exec",
        ),
        "dotnet": attr.label(
            doc = "",
            executable = True,
            cfg = "exec",
        ),
    },
    toolchains = ["@rules_dotnet//dotnet:toolchain_type"],
)
