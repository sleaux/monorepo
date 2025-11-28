load(":repositories.bzl", "p_register_toolchains")

_DEFAULT_NAME = "p"

def _toolchain_extension(module_ctx):
    registrations = {}
    for mod in module_ctx.modules:
        for toolchain in mod.tags.toolchain:
            if toolchain.name != _DEFAULT_NAME and not mod.is_root:
                fail("Only the root module may provide a name for the P toolchain.")

            if toolchain.name in registrations.keys():
                if toolchain.name == _DEFAULT_NAME:
                    # Prioritize the root-most registration of the default P toolchain version and
                    # ignore any further registrations (modules are processed breadth-first)
                    continue
                if toolchain.p_version == registrations[toolchain.name]:
                    # No problem to register a matching toolchain twice
                    continue
                fail("Multiple conflicting toolchains declared for name {} ({} and {})".format(
                    toolchain.name,
                    toolchain.p_version,
                    registrations[toolchain.name],
                ))
            else:
                registrations[toolchain.name] = toolchain.p_version
    for name, p_version in registrations.items():
        p_register_toolchains(
            name = name,
            p_version = p_version,
            register = False,
        )

p = module_extension(
    implementation = _toolchain_extension,
    tag_classes = {
        "toolchain": tag_class(attrs = {
            "name": attr.string(
                doc = "Base name for generated repositories",
                default = _DEFAULT_NAME,
            ),
            "p_version": attr.string(
                doc = "P version.",
            ),
        }),
    },
)
