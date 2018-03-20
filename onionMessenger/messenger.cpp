
#include "common.h"
#include "messenger.h"
#include "keymanager.h"

using namespace newkey;

namespace newmsger{

    Messenger::Messenger(string githubID, string passPhrase)
    {
        this->km = new Keymanager(githubID, passPhrase);
        if(km->Validation() == 1)
            CheckPW();
            exit(1);
    }

    Messenger::~Messenger(){}

    void recvKey(void) {

    }

    void recvMessage(void) {

    }

    void Messenger::Loop(){
        // create 3 threads
        // 1. recv new node info / node exit notification
        // 2. recv messages
        // 3. handle user input / print output to screen
        // 3rd thread will be handled in anohter module

        // 1st thread
        std::thread keyThread();

        // 2nd thread
        std::thread messageThread();
    }

}
