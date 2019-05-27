#!/bin/bash


# Starts MySQL
/etc/init.d/mysql start


mysql -u root -e "create database $mysql_database"; # create a mysql database
mysql -u root -e "USE mysql; UPDATE user SET plugin='mysql_native_password' WHERE User='root'; FLUSH PRIVILEGES;"

cd Bug_Collection # going to Bug_Collection folder
python3 Data_Populate.py -host localhost -database "$mysql_database" -user root # populating the database
cd .. # get back to the original folder

java -jar BugDatabase.jar "$mysql_database" root
