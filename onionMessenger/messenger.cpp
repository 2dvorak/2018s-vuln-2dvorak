
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

    Messenger::Messenger(){
        std::system("clear");
        CheckIP();

        string githubID = "";
        cout << "Your Github ID :";
        cin >> githubID;
        // check if valid githubID
        // NO COMMAND INJECTION ALLOWED EVER
        // "Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen" - Github
        if( CheckIDInvalid(githubID) ) {
            cout << "Invalid Github ID!" << endl;
            exit(1);
        }

        g_km = new Keymanager(githubID);
        g_km->SendKeyAlive();
        PGP_m = new PGPManager();
        PGP_m->ImportKeys(githubID);
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

    bool Messenger::CheckIDInvalid(string githubID) {
        // "Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen" - Github
        bool hyphenFound = false;
        const char* githubIDChar = githubID.c_str();
        if(githubIDChar[0] == '-' || githubIDChar[githubID.length() - 1] == '-') return true;
        for(int i = 0 ; i < githubID.length(); i++ ) {
            if(githubIDChar[i] < '0' || (githubIDChar[i] > '9' && githubIDChar[i] < 'A') || (githubIDChar[i] > 'Z' && githubIDChar[i] < 'a') || githubIDChar[i] > 'z') {
                if(githubIDChar[i] == '-') {
                    if(hyphenFound) {
                        return true;
                    }
                    else {
                        hyphenFound = true;
                    }
                }
                else {
                    return true;
                }
            }
        }
        return false;
    }
}
