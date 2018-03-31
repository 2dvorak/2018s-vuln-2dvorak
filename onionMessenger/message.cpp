
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
        struct timeval tp;
        gettimeofday(&tp, NULL);
        unsigned long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
        srand(ms);
        string randField = "", randData = "";
        unsigned int lenField = rand() % 32, lenData = rand() % 4096;
        for(unsigned int i = 0 ; i < lenField; i++) {
            randField.append(1,rand()%26 + 'a');
        }
        for(unsigned int i = 0 ; i < lenData; i++) {
            randData.append(1,rand()%26 + 'a');
        }
        this->jason[randField.c_str()] = randData;
        msgStr = tmp.dump();
    }

    void Message::EncMessage(string githubID){
        int cnt = g_km->ReturnCountMap();
        list<string> rnd_githubID = g_km->ReturnRndGithubID(githubID);
        std::vector<std::reference_wrapper<std::string>> v(rnd_githubID.begin(), rnd_githubID.end());
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(v.begin(), v.end(), generator);

        string tmp_content = PGP_m->Enc(this->jason.dump(), githubID);
        SetBridge(g_km->Findip(githubID), tmp_content);
        for(int i = 0; (i < cnt) && (i<5); i++){
            string tmp_githubID = v.back();
            v.pop_back();
            string tmp2_content = PGP_m->Enc(this->jason.dump(), tmp_githubID);
            SetBridge(g_km->Findip(tmp_githubID), tmp2_content);
        }


        // for debug
//        ofstream wf("test111.txt");
//        if(wf.is_open()){
//            wf << this->jason.dump();
//            wf.close();
//        }
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
