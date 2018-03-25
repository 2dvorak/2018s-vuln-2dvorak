
#include "common.h"
#include "messenger.h"
#include "keymanager.h"
#include "sockthread.h"
#include "onionui.h"
#include "pgpmanager.h"

using namespace newkey;
using namespace sockth;
using namespace oniui;
using namespace PGPCrypt;

namespace newmsger{

    Messenger::Messenger(string githubID, string passPhrase){
        CheckIP();
        g_km = new Keymanager(githubID, passPhrase);
        g_km->Validation();
        g_km->SendKeyAlive();
        PGP_m = new PGPManager();
    }

    Messenger::~Messenger(){
         g_km->SendKeyDie();
    }

    void Messenger::Main(){
        // create 3 threads
        // 1. recv new node info / node exit notification
        // 2. recv messages
        // 3. handle user input / print output to screen

        sockth = new Sockthread();
        sleep(1); // wait for recvThread to be created. let's not send before recv
        std::thread recvThread = sockth->RecvMessageThread();
        std::thread sendThread = sockth->SendMessageThread();
        std::thread *main = new std::thread(OnionUI::MainUI);
        // for now, no additional executions are left so we should
        // wait for the thread to return
        main->join();
    }
}
