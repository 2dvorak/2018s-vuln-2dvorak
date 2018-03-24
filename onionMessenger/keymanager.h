#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "common.h"
#include "message.h"

using namespace newmsg;

namespace newkey{
    class Nodeinfo;
    class Keymanager{
    public:
        Keymanager(string githubID, string passPhrase);
        virtual ~Keymanager();
        // Check validation between privatekey and passphrase
        bool Validation();
        // Search, Add, Delete node in unordered_map
        void AddMap(string githubID, Nodeinfo* IPnPubKey);
        void DelMap(string githubID);
        Nodeinfo* SearchMap(string githubID);
        // GetKey, PutKey Socket
        void GetKey();
        void SendKeyAlive();
        void SendKeyDie();
    private:
        // <githubID, *nodeInfo>
        unordered_map<string, Nodeinfo*>* nodeMap;
        unordered_map<string, Nodeinfo*>::iterator nodeIter;
        // This client's github ID and passphrase
        string githubID;
        string passPhrase;
        // My key JSON
        Message *myJSON;
    };

    class Nodeinfo{
    private:
        // Node info IP & Pubkey
        string ip;
        string pubkey;
    };
}

#endif // KEYMANAGER_H
