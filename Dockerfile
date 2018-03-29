# OnionMessenger

## Default Setting
FROM ubuntu:16.04
MAINTAINER zanywhale@kaist.ac.kr
WORKDIR /home/

## Setup some package
RUN sed -i 's/archive.ubuntu.com/ftp.daumkakao.com/g' /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y net-tools
RUN apt-get install -y netcat
RUN apt-get install -y g++
RUN apt-get install -y make
RUN apt-get install -y qt4-qmake
RUN apt-get install -y libncurses-dev
