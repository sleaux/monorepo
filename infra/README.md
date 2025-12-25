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

- Update `public_key = file("~/.ssh/tf_hetzner.pub")` in infra/ssh.tf
- Update `ssh_authorized_keys` in infra/user_data.yml

```bash
cd infra
export TF_VAR_hetzner_token=<YOUR-API-TOKEN> # or add -var='hetzner_token=<YOUR-API-TOKEN>'

terraform init
terraform apply
```

```bash
terraform destroy
```
