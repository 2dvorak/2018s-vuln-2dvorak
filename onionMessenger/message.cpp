
#include "common.h"
#include "message.h"

namespace newmsg{

    Message::Message(){}

    Message::Message(int id, bool bullian, string ip, string content) {
        this->id = id;
        this->bullian = bullian;
        this->ip = ip;
        this->content = content;
        this->jason["id"] = id;
        this->jason["bullian"] = bullian;
        this->jason["ip"] = ip;
        this->jason["content"] = content;
    }

    Message::Message(json jason) {
        this->jason["id"] = jason["id"];
        this->jason["bullian"] = jason["bullian"];
        this->jason["ip"] = jason["ip"];
        this->jason["content"] = jason["content"];
        this->id = jason["id"];
        this->bullian = jason["bullian"];
        this->ip = jason["ip"];
        this->content = jason["content"];
    }

    // KEY
    Message::Message(int id, int loginStatus, string ip, string githubID, string pubkey){
        this->jason["id"] = id;
        this->jason["bullian"] = loginStatus;
        this->jason["ip"] = ip;
        this->jason["githubID"] = githubID;
        this->jason["pubkey"] = pubkey;
    }

    Message::~Message(){}

    int Message::getID() {
        return this->id;
    }

    bool Message::getBullian() {
        return this->bullian;
    }

    string Message::getIP() {
        return this->ip;
    }

    string Message::getContent() {
        return this->content;
    }

    json Message::getJason() {
        return this->jason;
    }

    void Message::setID(int id) {
        this->id = id;
    }

    void Message::setBullian(int bullian) {
        this->jason["bullian"] = bullian;
    }

    void Message::setIP(string ip) {
        this->jason["ip"] = ip;
    }

    void Message::setContent(string content) {
        this->content = content;
    }

    void Message::setJason(json) {
        this->jason["id"] = jason["id"];
        this->jason["bullian"] = jason["bullian"];
        this->jason["ip"] = jason["ip"];
        this->jason["content"] = jason["content"];
    }

    void Message::SendMessage(){
        qSendMsg.push(this->jason.dump());
    }

    void Message::SendKey(){
        qSendMsg.push(this->jason.dump());
    }

    void Message::CheckMessage(){
        cout << this->jason.dump();
    }

    void Message::RecvMessage(){
        g_mutex.lock();
        string str(qRecvMsg.front());
        qRecvMsg.pop();
        g_mutex.unlock();
        this->jason = json::parse(str);
        this->id = this->jason["id"];
        this->bullian = this->jason["bullian"];
        this->ip = this->jason["ip"];
        this->content = this->jason["content"];
    }

}
