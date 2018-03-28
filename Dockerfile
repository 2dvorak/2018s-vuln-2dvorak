# OnionMessenger

## Default Setting
FROM ubuntu:16.04
MAINTAINER zanywhale@kaist.ac.kr
WORKDIR /home/

## Setup some package
RUN apt-get update
RUN apt-get install -y net-tools \
                       netcat \
