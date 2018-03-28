
#include "common.h"
#include "message.h"

namespace newmsg{

    Message::Message(){}

    // KEY
    Message::Message(string id, string loginStatus, string recvip, string githubID, string pubkey){
        this->jason["id"] = id;
        this->jason["bullian"] = loginStatus;
        this->jason["recvip"] = recvip;
        this->jason["sendip"] = MyIP;
        this->jason["githubID"] = githubID;
        this->jason["pubkey"] = pubkey;
    }

    Message::~Message(){}

    void Message::setIP(string ip) {
        this->jason["recvip"] = ip;
    }

    string Message::getIP(){
        return this->jason.at("recvip").get<std::string>();
    }

    void Message::setBullian(string bullian) {
        this->jason["bullian"] = bullian;
    }

    void Message::SetMessage(string githubID, string toip, string contents){
        this->jason["id"] = "1";
        this->jason["bullian"] = "1";
        this->jason["githubID"] = githubID;
        this->jason["recvip"] = toip;
        this->jason["sendip"] = MyIP;
        this->jason["content"] = contents;
    }

    void Message::SetBridge(string toip, string contents){
        this->jason["id"] = "1";
        this->jason["bullian"] = "0";
        this->jason["githubID"] = "";
        this->jason["recvip"] = toip;
        this->jason["sendip"] = "";
        this->jason["content"] = contents;
    }

    void Message::EncMessage(string githubID){
        int cnt = g_km->ReturnCountMap();
        string rnd_githubID = g_km->ReturnRndGithubID(githubID);
        string tmp_content = PGP_m->Enc(this->jason.dump(), rnd_githubID);
        SetBridge(g_km->Findip(rnd_githubID), tmp_content);
    }

    void Message::SendMessage(){
        s_mutex.lock();
        qSendMsg.push(this->jason.dump());
        s_mutex.unlock();
    }

    void Message::SendKey(){
        s_mutex.lock();
        qSendMsg.push(this->jason.dump());
        s_mutex.unlock();
    }

    void Message::CheckMessage(){
        cout << this->jason.dump() << endl;
    }

    void Message::RecvMessage(){
        r_mutex.lock();
        string str(qRecvMsg.front());
        qRecvMsg.pop();
        r_mutex.unlock();
//        this->jason = json::parse(str);
//        this->id = this->jason["id"];
//        this->bullian = this->jason["bullian"];
//        this->ip = this->jason["ip"];
//        this->content = this->jason["content"];
    }

}
