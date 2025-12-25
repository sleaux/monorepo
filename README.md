## Building

### Prerequisites

#### Bazel

```
wget https://github.com/bazelbuild/bazelisk/releases/download/v1.27.0/bazelisk-linux-amd64 -O bazelisk
chmod +x bazelisk
sudo mv bazelisk /usr/local/bin/bazel
```

### Build

```
bazel build //...
```

or

```
bazel test //...
```

### Format/Lint

```
bazel run format
bazel lint //...
```

### Editor integration

I am only using vscode for now.

```
# For clangd
bazel run @@hedron_compile_commands+//:refresh_all
```
