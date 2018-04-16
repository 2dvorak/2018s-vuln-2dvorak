
#include "common.h"
#include "keymanager.h"
#include "message.h"
#include <fstream>

namespace newkey{

    Keymanager::Keymanager(){}

    Keymanager::Keymanager(string githubID)
    {
        this->githubID = githubID;
        // get pub key file
        // TODO? get .pub from .key
        ifstream openFile1( (githubID+".pub").data() );
        if( openFile1.is_open()){
            openFile1.seekg(0, ios::end);
            int size = openFile1.tellg();
            this->pubkey.resize(size);
            openFile1.seekg(0, ios::beg);
            openFile1.read(&this->pubkey[0], size);
            openFile1.close();
        }
        else{
            cout << "Check github ID & pubkey plz :)" << endl;
            openFile1.close();
            exit(10);
        }

        nodeMap = new unordered_map<string, Nodeinfo*>;
        this->myJSON = new Message("0", "1", MyIP, this->githubID, this->pubkey);
    }

    Keymanager::~Keymanager(){

    }

    // manage node
    void Keymanager::AddMap(string githubID, Nodeinfo* IPnPubKey){
        // vector init
        vector<string>* newChatRoom = new std::vector<string>();
        chatRoomMap->insert(chatRoomMap->end(), pair<string, tuple<vector<string>*,unsigned int,time_t>*>(githubID, new tuple<vector<string>*,unsigned int,time_t>(newChatRoom, 0, 0)));
        //map addition
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

    string Keymanager::FindPubkey(string githubID){
        nodeIter = nodeMap->find(githubID);
        return nodeIter->second->pubkey;
    }

    string Keymanager::FindgithubID(string ip){
        for( nodeIter = nodeMap->begin(); nodeIter != nodeMap->end(); nodeIter++){
            if(ip.compare(nodeIter->second->ip) == 0)
                return nodeIter->first;
        }
        return "";
    }

    list<string> Keymanager::ReturnRndGithubID(string githubID){
        list<string> tmp_list;
        for( nodeIter = nodeMap->begin(); nodeIter != nodeMap->end(); nodeIter++){
            if( (githubID.compare(nodeIter->first) != 0) || (this->githubID.compare(nodeIter->first) != 0)){
                tmp_list.push_back(nodeIter->first);
            }
        }
        return tmp_list;
    }

    int Keymanager::ReturnCountMap(){
        return nodeMap->size();
    }

    string Keymanager::ReturnPubkey(){
        return this->pubkey;
    }

    string Keymanager::ReturnGithubID(){
        return this->githubID;
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
        cout << " ===================== " << endl;
        for( nodeIter = nodeMap->begin(); nodeIter != nodeMap->end(); nodeIter++){
            i++;
            cout << nodeIter->first << " : " << nodeIter->second->ip;
            if(DEBUG || DEMO) {
                cout << endl << "fpr : " << nodeIter->second->fpr;
            }
            cout << endl;
        }
        cout << " ===================== " << endl;
    }

    // manage node from packet
    void Keymanager::RecvKeyAlive(string jsonStr){
        json tmp;
        tmp = json::parse(jsonStr);
        string tmp_githubID = tmp.at("githubID").get<std::string>();
        string tmp_ip = tmp.at("sendip").get<std::string>();
        string tmp_fpr = tmp.at("fpr").get<std::string>();
        string tmp_pubkey = tmp.at("pubkey").get<std::string>();
        if(IsExist(tmp_githubID) == false){
            tmpInfo = new Nodeinfo(tmp_ip, tmp_fpr, tmp_pubkey);
            if(!(PGP_m->ImportPub(tmp_pubkey, tmp_fpr))) {
                if(DEBUG) {
                    cout << "malformed fpr and pubkey from " + tmp_githubID + "(" + tmp_ip + ")" << endl;
                }
                return;
            }
            AddMap(tmp_githubID, tmpInfo);
            this->SendKeyAlive(tmp_ip);
        } else {
            nodeIter = nodeMap->find(tmp_githubID);
            if(nodeIter != nodeMap->end()) {
                if(!(PGP_m->ImportPub(tmp_pubkey, tmp_fpr))) {
                    if(DEBUG) {
                        cout << "malformed fpr and pubkey from " + tmp_githubID + "(" + tmp_ip + ")" << endl;
                    }
                    return;
                }
                nodeIter->second->ip = tmp_ip;
                nodeIter->second->fpr = tmp_fpr;
                nodeIter->second->pubkey = tmp_pubkey;
            }
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

    void Keymanager::RecvKeyDieForever(string jsonStr){
        json tmp;
        tmp = json::parse(jsonStr);
        string tmp_githubID = tmp.at("githubID").get<std::string>();
        if(IsExist(tmp_githubID) == true){
            DelMap(tmp_githubID);
            chatRoomIter = chatRoomMap->find(tmp_githubID);
            if(chatRoomIter != chatRoomMap->end()) chatRoomMap->erase(chatRoomIter);
        }
    }

    // init
    void Keymanager::SendKeyAlive(){
        char buffer[5];
        this->myJSON->setBullian("1");
        this->myJSON->setFpr(PGP_m->getFpr());
        for( int ii = 0x00; ii < 10;ii++){
            sprintf(buffer, "%d",ii);
            this->myJSON->setIP(string("172.17.0.") + string(buffer));
            if( this->myJSON->getIP() != MyIP)
                this->myJSON->SendKey();
        }
    }

    // overload SendKeyAlive for specifying target
    void Keymanager::SendKeyAlive(string targetIP){
        this->myJSON->setBullian("1");
        this->myJSON->setIP(targetIP);
        this->myJSON->setFpr(PGP_m->getFpr());
        this->myJSON->SendKey();
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

    Nodeinfo::Nodeinfo(string a, string b, string c){
        this->ip = a;
        this->fpr = b;
        this->pubkey = c;
    }

    Nodeinfo::~Nodeinfo(){}

}
