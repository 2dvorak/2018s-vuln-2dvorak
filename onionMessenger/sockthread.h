#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H

#include "common.h"

namespace sockth{
    class Sockthread{
    public:
        Sockthread();
        virtual ~Sockthread();
        std::thread RecvMessageThread();
        std::thread SendMessageThread();
        static int CreateSendSocket();
        static int SendAll(int sockFd, string msgStr);
        static int CreateRecvSocket();
        static int RecvAll(int sockFd);
    private:
    };
}

#endif // SOCKTHREAD_H
