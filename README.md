## Building
### Prerequisites
#### Bazel
```
wget https://github.com/bazelbuild/bazelisk/releases/download/v1.27.0/bazelisk-linux-amd64 -O bazelisk
chmod +x bazelisk
sudo mv bazelisk /usr/local/bin/bazel
```

#### BuildBuddy remote execution (optional)
```
echo "build --remote_header=x-buildbuddy-api-key=$BUILD_BUDDY_API_KEY" > .bazelrc.buildbuddy
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

## Deploying
### Prequisites
https://community.hetzner.com/tutorials/setup-your-own-scalable-kubernetes-cluster#introduction

Getting Terraform
```bash
wget -O - https://apt.releases.hashicorp.com/gpg | sudo gpg --dearmor -o /usr/share/keyrings/hashicorp-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/hashicorp-archive-keyring.gpg] https://apt.releases.hashicorp.com $(grep -oP '(?<=UBUNTU_CODENAME=).*' /etc/os-release || lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/hashicorp.list
sudo apt update && sudo apt install terraform
```


Getting Kubectl
```bash
# If the folder `/etc/apt/keyrings` does not exist, it should be created before the curl command, read the note below.
# sudo mkdir -p -m 755 /etc/apt/keyrings
curl -fsSL https://pkgs.k8s.io/core:/stable:/v1.33/deb/Release.key | sudo gpg --dearmor -o /etc/apt/keyrings/kubernetes-apt-keyring.gpg
sudo chmod 644 /etc/apt/keyrings/kubernetes-apt-keyring.gpg # allow unprivileged APT programs to read this keyring
# This overwrites any existing configuration in /etc/apt/sources.list.d/kubernetes.list
echo 'deb [signed-by=/etc/apt/keyrings/kubernetes-apt-keyring.gpg] https://pkgs.k8s.io/core:/stable:/v1.33/deb/ /' | sudo tee /etc/apt/sources.list.d/kubernetes.list
sudo chmod 644 /etc/apt/sources.list.d/kubernetes.list   # helps tools such as command-not-found to work correctly
sudo apt-get update
sudo apt-get install -y kubectl
```

### Deploy
Optionally setup an ssh key for the created servers:
```
ssh-keygen -f ~/.ssh/tf_hetzner -t rsa -b 4096 -N ''
```
* Update `public_key = file("~/.ssh/tf_hetzner.pub")` in infra/ssh.tf
* Update `ssh_authorized_keys` in infra/user_data.yml



```bash
cd infra
export TF_VAR_hetzner_token=<YOUR-API-TOKEN> # or add -var='hetzner_token=<YOUR-API-TOKEN>'

terraform init
terraform apply
```

```bash
terraform destroy
```

## Roadmap
Basic paxos
* "I am the leader"

Things I want to do with this:
* Dynamic simulation testing
* P model to implementation checker, for model, tests, and continuous.
* Profiling and continuous profiling


Workspace setup todos:
* Easier clangd?
* Performance, binary size metrics
* Linting/formatting, enforce on commit
    * clang-format
    * clang-tidy - not quite right, keeps removing headers that are needed
    * https://raw.githubusercontent.com/cpplint/cpplint/HEAD/cpplint.py
    * lint/format as tests? seems nice. https://github.com/aspect-build/rules_lint/blob/129f3645b6f21b1f7c518ccf5e179aeb9ff720fc/example/tools/format/BUILD.bazel#L123

Deployment
* Finish terraform for infra
* Kubernetes or no?
* Zero-downtime deployment, rollback, etc. GitLab looks like it has some features here.

NPM and web
* I don't like having to `npx pnpm install --lockfile-only` outside of bazel.
* The whole npm/bazel/ibazel thing is not figured out.
