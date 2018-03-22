
#include "common.h"
#include "message.h"

namespace newmsg{

    Message::Message(){

    }

    Message::Message(int id, bool bullian, string IP, string content) {
        this->id = id;
        this->bullian = bullian;
        this->IP = IP;
        this->content = content;
        this->jason["id"] = id;
        this->jason["bullian"] = bullian;
        this->jason["IP"] = IP;
        this->jason["content"] = content;
    }

    Message::Message(json jason) {
        this->jason["id"] = jason["id"];
        this->jason["bullian"] = jason["bullian"];
        this->jason["IP"] = jason["IP"];
        this->jason["content"] = jason["content"];
        this->id = jason["id"];
        this->bullian = jason["bullian"];
        this->IP = jason["IP"];
        this->content = jason["content"];
    }

    Message::Message(string jsonStr) {
        this->jason = json::parse(jsonStr);
        this->id = this->jason["id"];
        this->bullian = this->jason["bullian"];
        this->IP = this->jason["IP"];
        this->content = this->jason["content"];
    }

    Message::~Message(){}

    int Message::getID() {
        return this->id;
    }

    bool Message::getBullian() {
        return this->bullian;
    }

    string Message::getIP() {
        return this->IP;
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

    void Message::setBullian(bool bullian) {
        this->bullian = bullian;
    }

    void Message::setIP(string IP) {
        this->IP = IP;
    }

    void Message::setContent(string content) {
        this->content = content;
    }

    void Message::setJason(json) {
        this->jason["id"] = jason["id"];
        this->jason["bullian"] = jason["bullian"];
        this->jason["IP"] = jason["IP"];
        this->jason["content"] = jason["content"];
    }

    void Message::SendMessage(){
        qSendMsg.push(this->jason.dump());
    }

    void Message::RecvMessage(){
        string str(qRecvMsg.front());
        qRecvMsg.pop();
        this->jason = json::parse(str);
        this->id = this->jason["id"];
        this->bullian = this->jason["bullian"];
        this->IP = this->jason["IP"];
        this->content = this->jason["content"];
    }

}
