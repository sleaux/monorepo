load("@aspect_rules_lint//lint:buf.bzl", "lint_buf_aspect")
load("@aspect_rules_lint//lint:clang_tidy.bzl", "lint_clang_tidy_aspect")

buf = lint_buf_aspect(
    config = Label("//:buf.yaml"),
)

clang_tidy = lint_clang_tidy_aspect(
    binary = Label("//tools/lint:clang_tidy"),
    configs = [
        Label("//:.clang-tidy"),
    ],
    verbose = False,
)
