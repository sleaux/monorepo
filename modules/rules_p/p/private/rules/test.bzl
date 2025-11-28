load("//p/private:providers.bzl", "PInfo", "PProjectRuntimeInfo")

def PTest(ctx):
    toolchain = ctx.toolchains["//p:toolchain_type"]
    p = toolchain.p_info.p
    p_executable = p[DefaultInfo].files_to_run.executable

    script = "\n".join([
        p_executable.short_path +
        " check " +
        ctx.attr.model[PProjectRuntimeInfo].dll +
        " --testcase " + ctx.attr.testcase +
        "  --outdir $TEST_UNDECLARED_OUTPUTS_DIR" +
        " ".join(ctx.attr.args) +
        " --seed " + str(ctx.attr.seed) +
        " $@",
    ])

    ctx.actions.write(
        output = ctx.outputs.executable,
        content = script,
    )
    runfiles = ctx.runfiles(ctx.files.model + p[DefaultInfo].default_runfiles.files.to_list() + p[DefaultInfo].data_runfiles.files.to_list())
    return [DefaultInfo(runfiles = runfiles)]

p_test = rule(
    implementation = PTest,
    test = True,
    attrs = {
        "model": attr.label(
            doc = "",
            providers = [PProjectRuntimeInfo],
        ),
        "testcase": attr.string(
            doc = "",
            mandatory = True,
        ),
        "seed": attr.int(
            doc = "",
            default = 0,
        ),
    },
    toolchains = ["//p:toolchain_type"],
)
