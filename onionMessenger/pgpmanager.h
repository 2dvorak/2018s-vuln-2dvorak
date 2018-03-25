#ifndef PGPMANAGER_H
#define PGPMANAGER_H

#include <iostream>
#include <gpgme.h>
using namespace std;

namespace PGPCrypt{
    
    class PGPManager{
    public:
        PGPManager();
        virtual ~PGPManager();
        void SetPubKey(string a_pubkey);
        void SetPriKey(string a_privkey);
        bool KeyValidation(string);
        string Enc(string input_plain);
        string Dec(string input_cipher);
    private:
        string privKey;
        string pubKey;
    };
}

#endif // PGPMANAGER_H
