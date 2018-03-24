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
        // for key
        Message(int id, int loginStatus, string IP, string githubID, string pubkey);
        virtual ~Message();
        // Socket Client
        void SendMessage();
        void RecvMessage();
        void SendKey();
        void RecvKey();
        int getID();
        bool getBullian();
        string getIP();
        string getContent();
        json getJason();
        void setID(int);
        void setBullian(int);
        void setIP(string);
        void setContent(string);
        void setJason(json);
        void CheckMessage();
    private:
        // We have to make rules(Protocol)
        // +a // string attachment;
        int id;
        int bullian;
        string ip;
        string content;
        json jason;
    };
}

#endif // MESSAGE_H
