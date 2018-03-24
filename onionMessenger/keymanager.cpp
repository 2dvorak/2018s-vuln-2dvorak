
#include "common.h"
#include "keymanager.h"
#include "message.h"

namespace newkey{

    Keymanager::Keymanager(){}

    Keymanager::Keymanager(string githubID, string passPhrase)
    {
        this->passPhrase = passPhrase;
        this->githubID = githubID;
        nodeMap = new unordered_map<string, Nodeinfo*>;
        this->myJSON = new Message("0", "1", MyIP, this->githubID, "string pubkey");
    }

    Keymanager::~Keymanager(){

    }

    bool Keymanager::Validation(){
        // check pw match private key
        return 0;
    }

    // manage node
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

    string Keymanager::Findip(string githubID){
        nodeIter = nodeMap->find(githubID);
        return nodeIter->second->ip;
    }

    bool Keymanager::IsExist(string githubID){
        nodeIter = nodeMap->find(githubID);
        if( nodeIter == nodeMap->end() )
            return false;
        else
            return true;
    }

    void Keymanager::ShowList(){
        int i = 0;
        cout << " === Your List! === " << endl;
        for( nodeIter = nodeMap->begin(); nodeIter != nodeMap->end(); nodeIter++){
            i++;
            cout << nodeIter->first << " : " << nodeIter->second->ip << endl;
        }
        cout << " ==== Done! ====" << endl;
    }

    // manage node from packet
    void Keymanager::RecvKeyAlive(string jsonStr){
        json tmp;
        tmp = json::parse(jsonStr);
        string tmp_githubID = tmp.at("githubID").get<std::string>();
        if(IsExist(tmp_githubID) == false){
            string tmp_ip = tmp.at("sendip").get<std::string>();
            string tmp_pubkey = tmp.at("pubkey").get<std::string>();
            tmpInfo = new Nodeinfo(tmp_ip, tmp_pubkey);
            AddMap(tmp_githubID, tmpInfo);
            this->SendKeyAlive();
        }
    }

    void Keymanager::RecvKeyDie(string jsonStr){
        json tmp;
        tmp = json::parse(jsonStr);
        string tmp_githubID = tmp.at("githubID").get<std::string>();
        if(IsExist(tmp_githubID) == true){
            DelMap(tmp_githubID);
        }
    }

    // init
    void Keymanager::SendKeyAlive(){
        char buffer[5];
        this->myJSON->setBullian("1");
        for( int ii = 0x00; ii < 10;ii++){
            sprintf(buffer, "%d",ii);
            this->myJSON->setIP(string("172.17.0.") + string(buffer));
            if( this->myJSON->getIP() != MyIP)
                this->myJSON->SendKey();
        }
    }

    // destructor
    void Keymanager::SendKeyDie(){
        char buffer[5];
        this->myJSON->setBullian("0");
        for( int ii = 0x00; ii < 10 ;ii++){
            sprintf(buffer, "%d",ii);
            this->myJSON->setIP(string("172.17.0.") + string(buffer));
            this->myJSON->SendKey();
        }
    }

    Nodeinfo::Nodeinfo(string a, string b){
        this->ip = a;
        this->pubkey = b;
    }

    Nodeinfo::~Nodeinfo(){}

}
