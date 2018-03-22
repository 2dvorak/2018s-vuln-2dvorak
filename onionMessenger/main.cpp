#include "common.h"
#include "messenger.h"
using namespace newmsger;

queue<string>qkey;
queue<string>qSendMsg;
queue<string>qRecvMsg;

int main(int argc, char *argv[])
{
    if( argc != 3 ){
        UsageMessenger();
    }
    else{
        Messenger *msger = new Messenger(argv[1], argv[2]);
        msger->Main();
    }
}
