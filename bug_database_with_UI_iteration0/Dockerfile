FROM ubuntu:18.04


ENV mysql_database "ipt_bugs"


COPY Bug_Collection   /IPT/UI/Bug_Collection
#COPY BugDatabase      /IPT/UI/BugDatabase
#COPY BugDatabase_lib      /IPT/UI/BugDatabase_lib 
COPY BugDatabase.jar  /IPT/UI/BugDatabase.jar
COPY setup.sh         /IPT/UI/setup.sh


WORKDIR /IPT/UI


RUN apt-get update && apt install mysql-server default-jre python3 python3-pip -y && pip3 install  mysql-connector && apt-get install xterm -y &&\
    chmod +x /IPT/UI/setup.sh


CMD ./setup.sh
