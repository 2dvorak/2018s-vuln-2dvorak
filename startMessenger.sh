docker run -v $(pwd):/home -w /home/onionMessenger -it onionmessenger /bin/sh -c '../setup.sh; ./onionMessenger'
docker rm -v $(docker ps -a -q -f status=exited)

