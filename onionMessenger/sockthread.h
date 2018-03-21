#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H

#include "common.h"

namespace sockth{
    class Sockthread{
    public:
        Sockthread();
        virtual ~Sockthread();
        std::thread recvMessageThread();
        //std::thread recvKeyThread();
        std::thread sendMessageThread();
        //std::thread sendKeyThread();
    private:
    };
}

#endif // SOCKTHREAD_H
