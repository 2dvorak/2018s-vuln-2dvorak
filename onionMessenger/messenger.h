#ifndef MESSENGER_H
#define MESSENGER_H

#include "common.h"
#include "keymanager.h"

using namespace newkey;

namespace newmsger{
    class Messenger{
    public:
        Messenger(string githubID, string passPhrase);
        virtual ~Messenger();
        void Loop();
        string msg;
    private:
        Keymanager *km;
    };
}

#endif // MESSENGER_H
