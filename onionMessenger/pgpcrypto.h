#ifndef PGPCRYPTO_H
#define PGPCRYPTO_H

#include "common.h"

namespace newcrypto{
    class Pgpcrypto{
    public:
        Pgpcrypto();
        virtual ~Pgpcrypto();
        string Enc();
        string Dec();
    private:
    };
}

#endif // PGPCRYPTO_H
