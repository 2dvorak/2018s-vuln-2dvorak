
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
                string str = qRecvMsg.front();
                cout << str << "\n";
                qRecvMsg.pop();
            }
        }
    }

    void uiSendThread() {
        //string str;
        while(1) {
            string str;
            printf("\n>");
            getline(cin, str);
            if(str.compare("/exit") == 0) {
                break;
            }
            qSendMsg.push(str);
        }
    }

    void uiDummyThread() {
        while(1) {
            string str;
            printf(">");
            //scanf("%s",&str[0]);
            getline(cin, str);
            if(str.compare("/exit") == 0) {
                break;
            }
            qSendMsg.push(str);
            //sleep(1);
            while(qRecvMsg.empty() == 1);
            string str2(qRecvMsg.front());
            qRecvMsg.pop();
            cout << str2 << endl;
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

        new std::thread(uiRecvThread);
        new std::thread(uiSendThread);
        //std::thread dumThread(uiDummyThread);

        // for now, no additional executions are left so we should
        // wait for the thread to return
        //uiSend.join();
        //uiRecv.join();
        recvThread.join();
        sendThread.join();
    }

}
