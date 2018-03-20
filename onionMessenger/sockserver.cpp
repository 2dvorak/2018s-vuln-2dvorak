
#include "common.h"
#include "sockserver.h"

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace socksv{

    Sockserver::Sockserver(){}

    Sockserver::~Sockserver(){}

    int recvKey(int sockFd, int portNum) {
         char buffer[256];
         int n;
         memset(buffer, 256, 0);
         n = read(sockFd,buffer,255);
         if (n < 0) {
             //error("ERROR reading from socket");
             return -1;
         }
         printf("Here is the message: %s\n",buffer);

         close(sockFd);

         return 0;
    }

    int createSocket() {
        int sockFd, newSockFd, portNum;
        socklen_t clientLen;
        char buffer[256];
        struct sockaddr_in servAddr, cliAddr;
        int n;

        // !!!!!!!!!!!!!!!!temporary port num
        portNum = 9987;

        // create socket
        // socket(int domain, int type, int protocol)
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockFd < 0) {
            // error("Error Opening Socket");
            return -1;
        }

        // clear address structure
        memset((char*) &servAddr, sizeof(servAddr), 0);

        // !!!!!!!!!!!!!!!!!!!!! which protocol??
        servAddr.sin_family = AF_INET;

        // !!!!!!!!!!!!!!!!!!!!! port number to what?
        servAddr.sin_port = htons(portNum);

        // !!!!!!!!!!!!!!!!!!!!! INADDR_ANY safe?
        // should be INADDR_ANY but how about checking if that
        // ip's in my node list
        // if not, ping to that node and check if valid node?
        servAddr.sin_addr.s_addr = INADDR_ANY;

        // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
        // bind socket to current IP address on portNum
        if (bind(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
            //error("ERROR on binding");
            return -1;
        }

        // listen() function put all new connections into backlog queue until accept()
        // !!!!!!!!!!!!!!!! SOMAXCONN ok?
        listen(sockFd, SOMAXCONN);

        // ? what for..?
        clientLen = sizeof(cliAddr);

        // accept(int s, struct sockaddr *addr, socklen_t *addrlen);
        // accept() function will write the connecting client's info
        // into address structure then returns a new socket file descriptor
        // for the accepted connection.
        while(1) {
            char clientIp[INET_ADDRSTRLEN];
            newSockFd = accept(sockFd, (struct sockaddr *) &cliAddr, &clientLen);
            if (newSockFd < 0) {
                //error("ERROR on accept");
                return -1;
            }
            if(inet_ntop(AF_INET, &(cliAddr.sin_addr), clientIp, INET_ADDRSTRLEN) == NULL ) {
                //perror("ERROR on inet_ntop");
                return -1;
            }
            printf("server: got connection from %s port %d\n", clientIp, ntohs(cliAddr.sin_port));
            std::thread recvKeyThread(recvKey, newSockFd, portNum);
        }

        // send 13 byte to new socket
        //send(newSockFd, "Hello, world!\n", 13, 0);

        memset(buffer,256,0);

        close(sockFd);
        return 0;
    }

    std::thread Sockserver::KeyServer(){

    }

    std::thread Sockserver::MessageServer(){}

}
