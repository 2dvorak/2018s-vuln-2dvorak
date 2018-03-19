#ifndef MESSENGER_H
#define MESSENGER_H

#include "common.h"
#include "keymanager.h"

using namespace newkey;

namespace newmsger{
    class Messenger{
    public:
        Messenger(string passPhrase);
        virtual ~Messenger();
        Keymanager *km;
        void Loop();
    private:

    };
}

#endif // MESSENGER_H
