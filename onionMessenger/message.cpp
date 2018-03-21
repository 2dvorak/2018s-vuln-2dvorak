
#include "common.h"
#include "message.h"

namespace newmsg{

    Message::Message(){

    }

    Message::Message(string msg) {
        this->msg = msg;
    }

    Message::~Message(){}

    void Message::SendMessage(){
        qSendMsg.push(this->msg);
    }

    void Message::RecvMessage(){
        this->msg = qRecvMsg.front();
        qRecvMsg.pop();
    }

}
