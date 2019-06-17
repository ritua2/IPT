docker run --env="DISPLAY" -e mysql_database=ipt_bugs -e mysql_password="samplepassword"  --net=host --volume="/root/.Xauthority:/root/.Xauthority:ro" --name=krishan  -it --rm carlosred/ipt_bugs-database:latest bash

