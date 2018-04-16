
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

    void Message::setFpr(string fpr) {
        this->jason["fpr"] = fpr;
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
        if(!DEBUG) {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            unsigned long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
            srand(ms);
            string randData = "";
            unsigned int lenData = rand() % 4096;
            for(unsigned int i = 0 ; i < lenData; i++) {
                randData.append(1,rand()%26 + 'a');
            }
            this->jason["padding"] = randData;
        }
    }

    void Message::EncMessage(string githubID){
        int cnt = g_km->ReturnCountMap();
        list<string> rnd_githubID = g_km->ReturnRndGithubID(githubID);
        std::vector<std::reference_wrapper<std::string>> v(rnd_githubID.begin(), rnd_githubID.end());
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(v.begin(), v.end(), generator);

        string tmp_content = PGP_m->Enc(this->jason.dump(), nodeMap->find(githubID)->second->fpr);
        SetBridge(g_km->Findip(githubID), tmp_content);
        for(int i = 0; i < cnt; i++){
            string tmp_githubID = v.back();
            v.pop_back();
            string tmp2_content = PGP_m->Enc(this->jason.dump(), nodeMap->find(tmp_githubID)->second->fpr);
            SetBridge(g_km->Findip(tmp_githubID), tmp2_content);
        }

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
    }

}
