#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"

using namespace std;
using json = nlohmann::json;

namespace newmsg{
    class Message{
    public:
        Message();
        // for key
        Message(string id, string loginStatus, string recvip, string githubID, string pubkey);
        virtual ~Message();
        // Socket Client
        void EncMessage(string githubID);
        void SendMessage();
        void RecvMessage();
        void SendKey();
        void RecvKey();

        // json setting
        void setBullian(string);
        void setIP(string);
        string getIP();
        void SetMessage(string, string, string);
        void SetBridge(string toip, string contents);
        void setJason(json);
        void CheckMessage();
    private:
        // We have to make rules(Protocol)
        // +a // string attachment;
        json jason;
    };
}

#endif // MESSAGE_H
