# 0th iteration of the database of logical errors.

Creation of Docker image, integration: Carlos Redondo

GUI: Krishan Pal Singh and Saumya Shah (https://github.com/saumyashah7/Repository-Lookup-Desktop)  

Curation of errors and integration: Trung Nguyen

Design, bug repo curation, integration: Ritu Arora



## Prequisites

1. Docker: Installation instructions for Linux (Ubuntu), Mac, and Windows are available [here](./docker_installation_instructions.md)






## Steps for installation (set-up):

```bash
apt install mysql-server # Install mysql
mysql -u root -e "create database <database>"; # create a mysql database
mysql -u root -e "USE mysql; UPDATE user SET plugin='mysql_native_password' WHERE User='root'; FLUSH PRIVILEGES;" # setting up root user
apt install default-jre # install jdk
apt-get install python3 # install python3
apt install python3-pip # install pip3
pip3 install  mysql-connector # install mysql-connector library
apt-get install xterm # install xterm
cd Bug_Collection # going to Bug_Collection folder
python3 Data_Populate.py -host localhost -database <database> -user root -passwd <password (if applicable)> # populating the database
cd .. (get back to the original folder)

# Steps for using/running the code:
java -jar BugDatabase.jar <database-name> root <passwd (if applicable)>
```


## Installation with Docker (GUI only)

```bash
git clone https://github.com/ritua2/IPT
cd IPT/bug_database_with_UI_iteration0


# Build the image
docker build -t carlosred/ipt_bugs-database:gui -f Dockerfile.gui .


# Linux systems
docker run --env="DISPLAY" -e mysql_database=ipt_bugs -e mysql_password="samplepassword"  --net=host --volume="$HOME/.Xauthority:/root/.Xauthority:ro" --rm carlosred/ipt_bugs-database:gui


# Note, if using root:
docker run --env="DISPLAY" -e mysql_database=ipt_bugs -e mysql_password="samplepassword"  --net=host --volume="/root/.Xauthority:/root/.Xauthority:ro" --rm carlosred/ipt_bugs-database:gui


# Mac systems
IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
xhost + $IP
docker run -e DISPLAY=$IP:0 -v /tmp/.X11-unix:/tmp/.X11-unix -e mysql_database=ipt_bugs --net=host --volume="$HOME/.Xauthority:/root/.Xauthority:ro" --rm carlosred/ipt_bugs-database:gui
```




## Installation with Docker (MPI)

```bash
git clone https://github.com/ritua2/IPT
cd IPT/bug_database_with_UI_iteration0


# Build the image
docker build -t carlosred/ipt_bugs-database:mpi -f Dockerfile.mpi .


# Linux systems
docker run -it --rm carlosred/ipt_bugs-database:mpi
```


## Installation with Docker (CUDA)

```bash
git clone https://github.com/ritua2/IPT
cd IPT/bug_database_with_UI_iteration0


# Build the image
docker build -t carlosred/ipt_bugs-database:cuda -f Dockerfile.cuda .


# Linux systems
docker run -it --rm carlosred/ipt_bugs-database:cuda
```



**Note**: If running this application on a cloud system, ensure that the login is done using ssh -X or similar:

```bash
ssh -X username@server.example.com
```


### Acknowledgements

Some of the bugs presented in this project have been obtained from Stack Overflow threads and we are grateful to all the contributors there. This project is funded through the National Science Foundation Award #1642396 and we are grateful to NSF for the same. We are also grateful to XSEDE (NSF award # 1548562) and TACC for providing the computational resources required for this project. 


### License

This software is being made available to the public under the new BSD license.

## Some useful links:

1. The Docker image containing the GUI and some of the errors/bugs: https://lnkd.in/em_qnYm

2. A video-demo of using this Docker image: https://lnkd.in/esADixF

3. Git repository containing the bugs: https://lnkd.in/enN7p79

4. Demo of installing the Docker image: https://lnkd.in/eTSm_mC

5. If interested in contributing errors/bugs, please create a pull request on the following Git repo:  https://lnkd.in/eQcKfZq 

