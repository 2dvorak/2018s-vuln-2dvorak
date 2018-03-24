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
        Keymanager();
        Keymanager(string githubID, string passPhrase);
        virtual ~Keymanager();
        // Check validation between privatekey and passphrase
        bool Validation();

        // Search, Add, Delete node in unordered_map
        void AddMap(string githubID, Nodeinfo* IPnPubKey);
        void DelMap(string githubID);
        Nodeinfo* SearchMap(string githubID);
        bool IsExist(string githubID);
        void ShowList();

        // GetKey, PutKey Socket
        void RecvKeyAlive(string jsonStr);
        void RecvKeyDie(string jsonStr);
        void SendKeyAlive();
        void SendKeyDie();
    private:
        // This client's github ID and passphrase
        string githubID;
        string passPhrase;
        // My key JSON
        Message *myJSON;
        Nodeinfo *tmpInfo;
    };

    class Nodeinfo{
    public:
        Nodeinfo(string, string);
        virtual ~Nodeinfo();
        string ip;
        string pubkey;
    private:
        // Node info IP & Pubkey
    };
}

#endif // KEYMANAGER_H
