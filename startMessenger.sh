docker run -v $(pwd):/home -it onionmessenger /bin/bash
docker rm -v $(docker ps -a -q -f status=exited)

