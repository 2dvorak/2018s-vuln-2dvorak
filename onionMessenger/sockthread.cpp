
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
        struct sockaddr_in servAddr;
        const char* msg = msgStr.c_str();

        json tmp;
        tmp = json::parse(msgStr);
        string destIP = tmp.at("recvip").get<std::string>();


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
        return 0;
    }

    int Sockthread::RecvAll(int sockFd) {
        char buffer[4096];
        int n;
        memset(buffer, '\x00', 4096);
        n = read(sockFd, buffer, 4096);
        if(n < 0) {
            perror("[KEY thread] Error reading socket\n");
            return -1;
        }
        string msgStr(buffer);
        json tmp;
        tmp = json::parse(msgStr);
        string tmp_id = tmp.at("id").get<std::string>();
        string tmp_bullian = tmp.at("bullian").get<std::string>();
        if( (tmp_id.compare("0") == 0) && tmp_bullian.compare("1") == 0) { // key alive
            r_mutex.lock();
            g_km->RecvKeyAlive(msgStr);
            r_mutex.unlock();
        }
        else if( (tmp_id.compare("0") == 0) && tmp_bullian.compare("0") == 0){ // key die
            r_mutex.lock();
            g_km->RecvKeyDie(msgStr);
            r_mutex.unlock();
        }
        else if( (tmp_id.compare("1") == 0) && (tmp_bullian.compare("1") == 0)){ // my message
            r_mutex.lock();
            qRecvMsg.push(msgStr);
            r_mutex.unlock();
        }
        else if( (tmp_id.compare("1") == 0) && (tmp_bullian.compare("0") == 0)){ // not my message

        }
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

        memset((char*) &servAddr, '\x00', sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(9987); // port number
        servAddr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
            perror("ERROR on binding");
            return -1;
        }

        clientLen = sizeof(cliAddr);
        while(1) {
            char clientIp[INET_ADDRSTRLEN];
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
        while(1){
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
