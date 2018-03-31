
#include "common.h"
#include "sockthread.h"
#include <sys/select.h>

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
        //const char* msg = msgStr.c_str();
        int bufSize = 4096;
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
        unsigned int iter = msgStr.length() / bufSize;
        if(msgStr.length() % bufSize > 0) iter++;
        for(unsigned int i = 0 ; i < iter ;i++) {
            n = write(sockFd, msgStr.substr(i*bufSize,bufSize).c_str(), bufSize);
            if( n < 0 ) {
                perror("ERROR writing msg to socket\n");
                cout << "ERROR writing msg to socket\n";
                close(sockFd);
                return -1;
            }
        }
        close(sockFd);
        return 0;
    }

    int Sockthread::RecvAll(int sockFd) {
        char buffer[4096];
        int n;
        int bufSize = 4096;
        memset(buffer, '\x00', bufSize);
        string msgStr = "";
        //struct timeval timeout;
        //int timeout = 1 * 1000;
        //setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        while((n = read(sockFd, buffer, bufSize)) > EOF) {
            msgStr += string(buffer);
            if(msgStr.find("\"}") != string::npos) break;
            memset(buffer, '\x00', bufSize);
        }
        /*if(msgStr.find("\"}") != string::npos) {
            close(sockFd);
        }*/
        /*fd_set set;
        FD_ZERO(&set); // clear the set
        FD_SET(sockFd, &set); // add our file descriptor to the set
        timeout.tv_sec = 0;
        timeout.tv_usec = 5000;
        while(true) {
            int rv = select(sockFd + 1, &set, NULL, NULL, &timeout);
            if (rv < 0)
            {
                // select error...
                return -1;
            }
            else if (rv == 0)
            {
                // timeout, socket does not have anything to read
                if(msgStr.find("\"}") != string::npos) {
                    // error or rogue message
                    break;
                }
            }
            else
            {
                // socket has something to read
                n = recv(sockFd, buffer, bufSize, 0);
                if (n < 0)
                {
                    // read failed...
                    return -1;
                }
                else if (n == 0)
                {
                    // peer disconnected...
                    return -1;
                }
                else
                {
                    // read successful...
                    msgStr += string(buffer);
                    if(msgStr.find("\"}") != string::npos) {
                        break;
                    }
                }
                memset(buffer, '\x00', bufSize);
            }
        }*/
        /*while((n = read(sockFd, buffer, 4)) >= 0) {
            if(n < 0) {
                perror("[KEY thread] Error reading socket\n");
                return -1;
            }
            if(n == 0) {
                // check if json ended?
                if(msgStr.find("}") != string::npos) {
                    // error or rogue message
                    break;
                }
            }
            if(n > 0) {
                msgStr += string(buffer);
                if(msgStr.find("}") != string::npos) {
                    break;
                }
            }
            memset(buffer, '\x00', 4);
        }*/
        if(msgStr.find("}") == string::npos) {
            // error or rogue message
            close(sockFd);
            return -1;
        } else {
            msgStr = msgStr.substr(0, msgStr.find("\"}") + 2);
            close(sockFd);
        }
        cout << msgStr << endl;
        json tmp;
        json tmp2;
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
        else if( (tmp_id.compare("1") == 0) ){
            string tmp_content = PGP_m->Dec(tmp.at("content").get<std::string>());
            tmp2 = json::parse(tmp_content);
            string tmp2_bullian = tmp2.at("bullian").get<std::string>();
            if( tmp2_bullian.compare("1") == 0){ // my message
                //std::unique_lock<std::mutex> lck(r_mutex);
                r_mutex.lock();
                //original enqueue
                //qRecvMsg.push(tmp_content);
                string tmp2_sender = tmp2.at("githubID").get<std::string>();
                string tmp2_content = tmp2.at("content").get<std::string>();
                map<string,tuple<vector<string>*,unsigned int,time_t>*>::iterator it = chatRoomMap->find(tmp2_sender);
                time_t now = time(NULL);
                if(it == chatRoomMap->end()) {
                    vector<string>* newChatRoom = new std::vector<string>();
                    // how about implementing something like g_km->AddMap();?

                    // always insert to begin.
                    // ACTUALLY, ALWAYS INSERT TO END, THEN ITERATE BACKWARDS.
                    newChatRoom->push_back(tmp2_sender + ": " + tmp2_content);
                    chatRoomMap->insert(pair<string, tuple<vector<string>*,unsigned int,time_t>*>(tmp2_sender, new tuple<vector<string>*,unsigned int,time_t>(newChatRoom, 0, now)));
                } else {
                    get<0>(*(it->second))->push_back(tmp2_sender + ": " + tmp2_content);
                    get<2>(*(it->second)) = now;
                }
                r_mutex.unlock();
                //r_cv.notify_one();
            }
            else if( tmp2_bullian.compare("0") == 0){ // not my message
                s_mutex.lock();
                qSendMsg.push(tmp_content);
                s_mutex.unlock();
            }
        }

        //close(sockFd);
        return 0;
    }

    int Sockthread::CreateRecvSocket() {
        int sockFd, newSockFd;
        socklen_t clientLen;
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
