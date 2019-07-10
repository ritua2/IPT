IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
xhost + $IP
docker run -e DISPLAY=$IP:0 -v /tmp/.X11-unix:/tmp/.X11-unix -e mysql_database=ipt_bugs --net=host --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --rm carlosred/ipt_bugs-database:latest
