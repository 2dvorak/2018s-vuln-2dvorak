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
        Keymanager(string githubID);
        virtual ~Keymanager();

        // Search, Add, Delete node in unordered_map
        void AddMap(string githubID, Nodeinfo* IPnPubKey);
        void DelMap(string githubID);
        Nodeinfo* SearchMap(string githubID);
        string Findip(string githubID);
        string FindPubkey(string githubID);
        string FindgithubID(string ip);
        int ReturnCountMap();
        list<string> ReturnRndGithubID(string githubID);
        string ReturnPubkey();
        string ReturnGithubID();
        bool IsExist(string githubID);
        void ShowList();

        // GetKey, PutKey Socket
        void RecvKeyAlive(string jsonStr);
        void RecvKeyDie(string jsonStr);
        void SendKeyAlive();
        void SendKeyDie();
        void SendKey(string ip);

    private:
        // This client's github ID and passphrase
        string githubID;
        string passPhrase;
        string pubkey;
        string privkey;
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
