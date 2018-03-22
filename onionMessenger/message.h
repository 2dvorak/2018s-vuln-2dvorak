#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"

namespace newmsg{
    class Message{
    public:
        Message();
        Message(int, bool, string, string);
        Message(json);
        Message(string);
        virtual ~Message();
        // Socket Client
        void SendMessage();
        void RecvMessage();
        int getID();
        bool getBullian();
        string getIP();
        string getContent();
        json getJason();
        void setID(int);
        void setBullian(bool);
        void setIP(string);
        void setContent(string);
        void setJason(json);
    private:
        // We have to make rules(Protocol)
        // +a // string attachment;
        int id;
        bool bullian;
        string IP;
        string content;
        json jason;
    };
}

#endif // MESSAGE_H
