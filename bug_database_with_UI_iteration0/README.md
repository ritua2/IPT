# 0th iteration of the database of logical errors.

Curation of errors and integration: Trung Nguyen

UI: Saumya Shah (https://github.com/saumyashah7/Repository-Lookup-Desktop)

Creation of Docker image: Carlos Redondo

Design and bug repo curation: Ritu Arora


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

# Build the image
docker build -t ipt/bugs-database:latest .

# Change the name of the bug database with mysql_database
docker run --env="DISPLAY" -e mysql_database=ipt_bugs -e mysql_password=password1 --net=host --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --rm ipt/bugs-database:latest
```


