#ifndef NEWPROTO_H
#define NEWPROTO_H

#include "common.h"

namespace newpro{
    class Newproto{
    public:
        Newproto();
        virtual ~Newproto();
        void hello();
        void goodBye();
    private:
    };
}

#endif // NEWPROTO_H
