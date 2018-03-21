
#include "common.h"
#include "messenger.h"
#include "keymanager.h"
#include "sockthread.h"

using namespace newkey;
using namespace sockth;

namespace newmsger{

    Messenger::Messenger(string githubID, string passPhrase)
    {
        this->km = new Keymanager(githubID, passPhrase);
        if(km->Validation() == 1) {
            CheckPW();
            exit(1);
        }
    }

    Messenger::~Messenger(){}

    void uiRecvThread() {
        while(1) {
            if(qRecvMsg.empty() == 0) {
                str = qRecvMsg.front();
                qRecvMsg.pop();
                printf(str);
                printf("\n");
            }
        }
    }

    void uiSendThread() {
        string str;
        printf("\n>");
        while(1) {
            scanf("%s",str);
            if(str.compare("/exit") == 0) {
                break;
            }
            qSendMsg.push(str);
        }
    }

    void Messenger::Loop(){
        // create 3 threads
        // 1. recv new node info / node exit notification
        // 2. recv messages
        // 3. handle user input / print output to screen
        // 3rd thread will be handled in anohter module

        Sockthread *sockth = new Sockthread();
        std::thread recvThread = sockth->recvMessageThread();
        // wait for recvThread to be created. let's not send before recv
        sleep(1);
        std::thread sendThread = sockth->sendMessageThread();

        std::thread uiSend(uiSendThread);
        std::thread uiRecv(uiRecvThread);

        // for now, no additional executions are left so we should
        // wait for the thread to return
        uiSend.join();
        uiRecv.join();
        recvThread.join();
        sendThread.join();
    }

}
