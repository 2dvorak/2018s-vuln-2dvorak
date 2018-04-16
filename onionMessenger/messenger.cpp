
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
        std::system("/usr/bin/clear");
        CheckIP();

        string githubID = "";
        cout << "Your Github ID :";
        cin >> githubID;
        if( CheckIDInvalid(githubID) ) {
            cout << "Invalid Github ID!" << endl;
            exit(1);
        }
        chatRoomMap = new unordered_map<string, tuple<vector<string>*, unsigned int, time_t>*>;
        g_km = new Keymanager(githubID);
        PGP_m = new PGPManager();
        PGP_m->ImportKeys(githubID);
        g_km->SendKeyAlive();
    }

    Messenger::~Messenger(){
         g_km->SendKeyDie();
    }

    void Messenger::Main(){
        sockth = new Sockthread();
        sleep(1);
        std::thread recvThread = sockth->RecvMessageThread();
        std::thread sendThread = sockth->SendMessageThread();
        std::thread *main = new std::thread(OnionUI::MainUI);
        main->join();
    }

    bool Messenger::CheckIDInvalid(string githubID) {
        // "Username may only contain alphanumeric characters or single hyphens, and cannot begin or end with a hyphen" - Github
        bool hyphenFound = false;
        const char* githubIDChar = githubID.c_str();
        if(githubIDChar[0] == '-' || githubIDChar[githubID.length() - 1] == '-') return true;
        for(unsigned int i = 0 ; i < githubID.length(); i++ ) {
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
