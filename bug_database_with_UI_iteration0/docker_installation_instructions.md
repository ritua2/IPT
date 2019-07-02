# Instructions for Docker installation

-------------



## Linux (Ubuntu, 16.04 or above)

```bash
#!/bin/bash

# Installs docker and erases the previous version

apt-get remove docker docker-engine docker.io
apt-get update

apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common -y

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"

apt-get update
apt-get install docker-ce -y

printf "Succesfully installed docker\n"
```


The official Docker-CE installation guide is available [here](https://docs.docker.com/install/linux/docker-ce/ubuntu/).



## Mac

Follow Docker Desktop installation guide for [Mac](https://docs.docker.com/docker-for-mac/install/).



## Windows

Follow Docker Desktop installation guide for [Windows](https://docs.docker.com/docker-for-windows/install/).

