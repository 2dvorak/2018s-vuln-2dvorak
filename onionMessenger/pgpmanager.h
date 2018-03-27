#ifndef PGPMANAGER_H
#define PGPMANAGER_H

#include <iostream>
using namespace std;

namespace PGPCrypt{

    class PGPManager{
    public:
        PGPManager();
        virtual ~PGPManager();
        string Enc(string input_plain, string recipientID);
        string Dec(string input_cipher);
        void ImportKeys();
        void SetTTYEcho(bool enable);
    private:
        string passPhrase;
    };
}

#endif // PGPMANAGER_H
