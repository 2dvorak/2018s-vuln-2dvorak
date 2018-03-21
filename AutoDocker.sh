docker build -t onionmessenger .
docker run -it onionmessenger /bin/bash
docker rm -v $(docker ps -a -q -f status=exited)

