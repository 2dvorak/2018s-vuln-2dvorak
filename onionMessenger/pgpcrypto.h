#ifndef PGPCRYPTO_H
#define PGPCRYPTO_H

#include "common.h"
using namespace std;

namespace newcrypto{
    class Pgpcrypto{
    public:
        Pgpcrypto();
        virtual ~Pgpcrypto();
        string enc();
        string dec();
    private:
    };
}

#endif // PGPCRYPTO_H
