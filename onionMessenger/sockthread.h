#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H

#include "common.h"

namespace sockth{
    class Sockthread{
    public:
        Sockthread();
        virtual ~Sockthread();
        std::thread recvMessageThread();
        std::thread sendMessageThread();
        static int createSendSocket();
        static int sendMessage(int sockFd, string msgStr);
        static int createRecvSocket();
        static int recvKey(int sockFd);
    private:
    };
}

#endif // SOCKTHREAD_H
