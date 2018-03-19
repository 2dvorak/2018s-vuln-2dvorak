#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
using namespace std;

namespace newmsg{
    class message{
    public:
        message();
        virtual ~message();
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
