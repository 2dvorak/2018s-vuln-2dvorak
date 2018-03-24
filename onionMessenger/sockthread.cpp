
#include "common.h"
#include "sockthread.h"

namespace sockth{

    Sockthread::Sockthread(){}

    Sockthread::~Sockthread(){}

    int Sockthread::SendAll(int sockFd, string msgStr) {
        // get message from buffer
        // getMesg();
        // get recv ip addr
        // getIp();

        int n;
        json tmp;
        struct sockaddr_in servAddr;
        const char* msg = msgStr.c_str();
        try{
        tmp = json::parse(msgStr);
        string destIP = tmp.at("ip").get<std::string>();
        memset((char *) &servAddr, '\x00', sizeof(servAddr));

            servAddr.sin_family = AF_INET;
            inet_pton(AF_INET, destIP.c_str(), &servAddr.sin_addr);
            servAddr.sin_port = htons(9987); // port number
            if( connect(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
                return -1;
            }
            n = write(sockFd, msg, strlen(msg));
            if( n < 0 ) {
                perror("ERROR writing msg to socket\n");
                return -1;
            }
            close(sockFd);
        }
        catch(int e){
            return 0;
        }
        return 0;
    }

    int Sockthread::RecvAll(int sockFd) {
        char buffer[256];
        int n;
        memset(buffer, '\x00', 256);
        n = read(sockFd,buffer,255);
        if(n < 0) {
            perror("[KEY thread] Error reading socket\n");
            return -1;
        }
        string msgStr(buffer);
        r_mutex.lock();
        qRecvMsg.push(msgStr);
        r_mutex.unlock();
        close(sockFd);

        return 0;
    }

    int Sockthread::CreateRecvSocket() {
        int sockFd, newSockFd;
        socklen_t clientLen;
        char buffer[256];
        struct sockaddr_in servAddr, cliAddr;

        // create socket
        // socket(int domain, int type, int protocol)
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockFd < 0) {
            perror("Error Opening Socket");
            return -1;
        }

        // clear address structure(needed?)
        memset((char*) &servAddr, '\x00', sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(9987); // port number

        // !!!!!!!!!!!!!!!!!!!!! INADDR_ANY safe?
        // should be INADDR_ANY but how about checking if that
        // ip's in my node list
        // if not, ping to that node and check if valid node?
        servAddr.sin_addr.s_addr = INADDR_ANY;

        // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
        // bind socket to current IP address on portNum
        if (bind(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
            perror("ERROR on binding");
            return -1;
        }

        // ? what for..?
        clientLen = sizeof(cliAddr);

        // accept(int s, struct sockaddr *addr, socklen_t *addrlen);
        // accept() function will write the connecting client's info
        // into address structure then returns a new socket file descriptor
        // for the accepted connection.
        while(1) {
            char clientIp[INET_ADDRSTRLEN];

            // listen() function put all new connections into backlog queue until accept()
            // !!!!!!!!!!!!!!!! SOMAXCONN ok?
            listen(sockFd, SOMAXCONN);

            newSockFd = accept(sockFd, (struct sockaddr *) &cliAddr, &clientLen);
            if (newSockFd < 0) {
                perror("ERROR on accept");
                return -1;
            }
            if(inet_ntop(AF_INET, &(cliAddr.sin_addr), clientIp, INET_ADDRSTRLEN) == NULL ) {
                perror("ERROR on inet_ntop");
                return -1;
            }
            new std::thread(Sockthread::RecvAll, newSockFd);
        }

        memset(buffer, '\x00', 256);

        close(sockFd);
        return 0;
    }

    int Sockthread::CreateSendSocket() {
        while(1) {
            // check msg queue
            while(qSendMsg.empty() == 1) ;
            int sockFd;

            sockFd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockFd < 0) {
                perror("ERROR opening socket");
                return -1;
            }
            s_mutex.lock();
            string msg(qSendMsg.front());
            qSendMsg.pop();
            s_mutex.unlock();
            new std::thread(Sockthread::SendAll, sockFd, msg);
        }
    }

    std::thread Sockthread::RecvMessageThread(){
        std::thread t(Sockthread::CreateRecvSocket);
        return t;
    }

    std::thread Sockthread::SendMessageThread(){
        std::thread t(Sockthread::CreateSendSocket);
        return t;
    }

}
