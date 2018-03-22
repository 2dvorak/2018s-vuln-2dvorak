
#include "common.h"
#include "messenger.h"
#include "keymanager.h"
#include "sockthread.h"
#include "onionui.h"

using namespace newkey;
using namespace sockth;
using namespace oniui;

namespace newmsger{

    Messenger::Messenger(string githubID, string passPhrase){
        this->km = new Keymanager(githubID, passPhrase);
        if(km->Validation() == 1) {
            CheckPW();
            exit(1);
        }
    }

    Messenger::~Messenger(){}

    void Messenger::Main(){
        // create 3 threads
        // 1. recv new node info / node exit notification
        // 2. recv messages
        // 3. handle user input / print output to screen

        sockth = new Sockthread();
        std::thread recvThread = sockth->recvMessageThread();
        // wait for recvThread to be created. let's not send before recv
        sleep(1);
        std::thread sendThread = sockth->sendMessageThread();

        new std::thread(OnionUI::MainUI);

        // for now, no additional executions are left so we should
        // wait for the thread to return
        recvThread.join();
        sendThread.join();
    }
}
