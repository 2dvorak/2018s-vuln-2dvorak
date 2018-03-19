#ifndef SOCKSERVER_H
#define SOCKSERVER_H

#include "common.h"

namespace socksv{
    class Sockserver{
    public:
        Sockserver();
        virtual ~Sockserver();
        std::thread KeyServer();
        std::thread MessageServer();
    private:
    };
}

#endif // SOCKSERVER_H
