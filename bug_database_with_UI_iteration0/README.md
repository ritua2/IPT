# 0th iteration of the database of logical errors.

Curation of errors and integration: Trung Nguyen

UI: Saumya Shah (https://github.com/saumyashah7/Repository-Lookup-Desktop)

Creation of Docker image, integration: Carlos Redondo

Design, bug repo curation, integration: Ritu Arora


## Steps for installation (set-up):

```bash
apt install mysql-server # Install mysql
mysql -u root -e "create database <database>"; # create a mysql database
mysql -u root -e "USE mysql; UPDATE user SET plugin='mysql_native_password' WHERE User='root'; FLUSH PRIVILEGES;" # setting up root user
apt install default-jre # install jdk
apt-get install python3 # install python3
apt install python3-pip # install pip3
pip3 install  mysql-connector # install mysql-connector library
apt-get install xterm # install xtearm
cd Bug_Collection # going to Bug_Collection folder
python3 Data_Populate.py -host localhost -database <database> -user root -passwd <password (if applicable)> # populating the database
cd .. (get back to the original folder)

# Steps for using/running the code:
java -jar BugDatabase.jar <database-name> root <passwd (if applicable)>
```



## Installation with Docker

```bash
git clone https://github.com/ritua2/IPT
cd IPT/bug_database_with_UI_iteration0


# Download the MySQL client for java and Ubuntu from https://dev.mysql.com/downloads/file/?id=485756
# Install it on the current directory

# Build the image
docker build -t carlosred/ipt_bugs-database:latest .


# Linux systems
docker run --env="DISPLAY" -e mysql_database=ipt_bugs -e mysql_password="samplepassword"  --net=host --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --rm carlosred/ipt_bugs-database:latest


# Note, if using root:
docker run --env="DISPLAY" -e mysql_database=ipt_bugs -e mysql_password="samplepassword"  --net=host --volume="/root/.Xauthority:/root/.Xauthority:rw" --rm carlosred/ipt_bugs-database:latest


# Mac systems
IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
xhost + $IP
docker run -e DISPLAY=$IP:0 -v /tmp/.X11-unix:/tmp/.X11-unix -e mysql_database=ipt_bugs --net=host --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --rm carlosred/ipt_bugs-database:latest
```


**Note**: If running this application on a cloud system, ensure that the login is done using ssh -X or similar:

```bash
ssh -X username@server.example.com
```

