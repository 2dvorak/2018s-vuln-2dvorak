#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "common.h"
#include "message.h"

using namespace newmsg;
using namespace std;
using json = nlohmann::json;

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
        void RecvKeyAlive(string json);
        void RecvKeyDie(string json);
        void SendKeyAlive();
        void SendKeyDie();
    private:
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
