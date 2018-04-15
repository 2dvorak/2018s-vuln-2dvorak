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
        bool ImportPub(string pub, string fpr);
        void ImportKeys(string githubID);
        string getFpr();
        bool VerifyFprAndPubkey(string recvedFpr, string importedKeyID);
        void SetTTYEcho(bool enable);
    private:
        string passPhrase;
        string fpr;
    };
}

#endif // PGPMANAGER_H
