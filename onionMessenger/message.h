#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"

namespace newmsg{
    class Message{
    public:
        Message();
        Message(string);
        virtual ~Message();
        // Socket Client
        void SendMessage();
        void RecvMessage();
        string msg;
    private:
        // We have to make rules(Protocol)
        // +a // string attachment;
    };
}

#endif // MESSAGE_H
