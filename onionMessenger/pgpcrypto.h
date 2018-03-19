#ifndef PGPCRYPTO_H
#define PGPCRYPTO_H

#include <iostream>
using namespace std;

namespace newcrypto{
    class pgpcrypto{
    public:
        pgpcrypto();
        virtual ~pgpcrypto();
        string enc();
        string dec();
    private:
    };
}

#endif // PGPCRYPTO_H
