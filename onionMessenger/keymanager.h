#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "common.h"

namespace newkey{
    class Nodeinfo;
    class Keymanager{
    public:
        Keymanager(string githubID, string passPhrase);
        virtual ~Keymanager();
        // Check validation between privatekey and passphrase
        bool Validation();
        // Search, Add, Delete node in unordered_map
        void SearchMap(string githubID);
        void AddMap(string githubID);
        void DelMap(string githubID);
        // GetKey, PutKey for request or response
        void GetKey();
        void PutKey();
    private:
        // <githubID, *nodeInfo>
        unordered_map<string, Nodeinfo*>* nodeMap;
        unordered_map<string, Nodeinfo*>::iterator nodeIter;
        // This client's github ID and passphrase
        string githubID;
        string passPhrase;
    };

    class Nodeinfo{
    private:
        // Node info IP & Pubkey
        string IP;
        string pubkey;
    };
}

#endif // KEYMANAGER_H
