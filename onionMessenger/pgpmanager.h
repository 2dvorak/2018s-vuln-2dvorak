#ifndef PGPMANAGER_H
#define PGPMANAGER_H

#include <iostream>
using namespace std;

namespace PGPCrypt{

    class PGPManager{
    public:
        PGPManager();
        virtual ~PGPManager();
        string Enc(string input_plain);
        string Dec(string input_cipher);
        void ImportKeys();
    private:
        string privKey;
        string pubKey;
    };
}

#endif // PGPMANAGER_H
