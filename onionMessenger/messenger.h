#ifndef MESSENGER_H
#define MESSENGER_H

#include "common.h"
#include "keymanager.h"
#include "sockthread.h"
#include "onionui.h"

using namespace sockth;
using namespace newkey;
using namespace oniui;

namespace newmsger{
    class Messenger{
    public:
        Messenger(string githubID, string passPhrase);
        virtual ~Messenger();
        void Main();
    private:
        Keymanager *km;
        Sockthread *sockth;
    };
}

#endif // MESSENGER_H
