#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"
using namespace std;

namespace newmsg{
    class Message{
    public:
        Message();
        virtual ~Message();
        string parseToJson();
        void sendMessage();
        void showMessage();
    private:
        string title;
        string to;
        string from;
        string contents;
        // string attachment;
    };
}

#endif // MESSAGE_H
