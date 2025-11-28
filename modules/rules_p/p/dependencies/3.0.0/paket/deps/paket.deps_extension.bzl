"Generated"

load(":paket.deps.bzl", _deps = "deps")

def _deps_impl(module_ctx):
    _deps()
    return module_ctx.extension_metadata(reproducible = True)

deps_extension = module_extension(
    implementation = _deps_impl,
)
