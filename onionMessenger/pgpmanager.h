#ifndef PGPMANAGER_H
#define PGPMANAGER_H

#include <iostream>
using namespace std;

namespace PGPCrypt{
    class PGPManager;

    class PGPManager{
    public:
        PGPManager();
        virtual ~PGPManager();
        bool KeyValidation(string);
        string Enc();
        string Dec();
    private:
    };
}

#endif // PGPMANAGER_H
