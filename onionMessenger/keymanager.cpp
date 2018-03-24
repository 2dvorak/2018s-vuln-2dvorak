
#include "common.h"
#include "keymanager.h"
#include "message.h"

namespace newkey{

    Keymanager::Keymanager(string githubID, string passPhrase)
    {
        this->passPhrase = passPhrase;
        this->githubID = githubID;
        this->nodeMap = new unordered_map<string, Nodeinfo*>;
        this->myJSON = new Message(0, 1, MyIP, this->githubID, "string pubkey");
    }

    Keymanager::~Keymanager(){}

    bool Keymanager::Validation(){
        // check pw match private key
        return 0;
    }

    void Keymanager::AddMap(string githubID, Nodeinfo* IPnPubKey){
        nodeMap->insert(unordered_map<string, Nodeinfo*>::value_type(githubID, IPnPubKey));
    }

    void Keymanager::DelMap(string githubID){
        nodeIter = nodeMap->find(githubID);
        delete(nodeIter->second);
        nodeMap->erase(githubID);
    }

    Nodeinfo* Keymanager::SearchMap(string githubID){
        nodeIter = nodeMap->find(githubID);
        return nodeIter->second;
    }

    void Keymanager::SendKeyAlive(){
        char buffer[5];
        this->myJSON->setBullian(1);
        for( int ii = 0x00; ii < 10;ii++){
            sprintf(buffer, "%d",ii);
            this->myJSON->setIP(string("172.17.0.") + string(buffer));
            this->myJSON->SendKey();
//            this->myJSON->CheckMessage(); cout << endl;// for debug
        }
    }

    void Keymanager::SendKeyDie(){
        char buffer[5];
        this->myJSON->setBullian(0);
        for( int ii = 0x00; ii < 255 ;ii++){
            sprintf(buffer, "%d",ii);
            this->myJSON->setIP(string("172.17.0.") + string(buffer));
        }
    }

}
