#ifndef NEWPROTO_H
#define NEWPROTO_H

#include "common.h"

namespace newpro{
    class Newproto{
    public:
        Newproto();
        virtual ~Newproto();
        void Hello();
        void GoodBye();
    private:
    };
}

#endif // NEWPROTO_H
