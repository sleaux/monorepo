PProjectCompileInfo = provider(
    doc = "All the required inputs to the P compiler",
    fields = {
        "name": "string: ",
        "input_files": "list[File] ",
    },
)

PProjectRuntimeInfo = provider(
    doc = "All the required inputs to run the P checker",
    fields = {
        "name": "string: ",
        "dll": "string ",
        "directory": "File ",
    },
)

PInfo = provider(
    doc = "Information about the p toolchain.",
    fields = {
        "p": "File ",
        "p_files": "list[File] ",
        "dotnet": "File ",
        "dotnet_files": "list[File] ",
    },
)
