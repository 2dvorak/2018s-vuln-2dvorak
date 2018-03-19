#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "common.h"
using namespace std;

namespace newkey{
    class Keymanager{
    public:
        Keymanager(string pP);
        virtual ~Keymanager();
        // Check validation between privatekey and passphrase
        bool Validation();
    private:
        // <githubID, IP>
        unordered_map<string, string>* nodeInfo;
        unordered_map<string ,string>::iterator nodeIter;
        string passPhrase;
    };
}


#endif // KEYMANAGER_H
