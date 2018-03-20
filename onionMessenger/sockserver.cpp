
#include "common.h"
#include "sockserver.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

namespace socksv{

    Sockserver::Sockserver(){}

    Sockserver::~Sockserver(){}

    int createSocket() {
        int sockFd, newSockFd, portNum;
        socklent_t clientLen;
        char buffer[256];
        struct sockaddr_in servAddr, cliAddr;
        int n;

        // create socket
        // socket(int domain, int type, int protocol)
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockFd < 0) {
            // error("Error Opening Socket");
            return -1;
        }

        // clear address structure
        bzero((char*) &servAddr, sizeof(servAddr));

        // !!!!!!!!!!!!!!!!!!!!! which protocol??
        servAddr.sin_family = AF_INET;

        // !!!!!!!!!!!!!!!!!!!!! port number to what?
        servAddr.sin_port = htons(portNum);

        // !!!!!!!!!!!!!!!!!!!!! INADDR_ANY safe?
        // should be INADDR_ANY but how about checking if that
        // ip's in my node list
        // if not, ping to that node and check if valid node?
        servAddr.sin_addr.s_addr = INADDR_ANY;

        return 0;
    }

    std::thread Sockserver::KeyServer(){

    }

    std::thread Sockserver::MessageServer(){}

}
