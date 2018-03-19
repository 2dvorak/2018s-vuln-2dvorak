
#include "common.h"
#include "messenger.h"
#include "keymanager.h"

using namespace newkey;
using namespace newmsger;

Messenger::Messenger(string passPhrase)
{
    this->km = new Keymanager(passPhrase);
    if(km->Validation() == 1)
        exit(1);
}

Messenger::~Messenger(){}

void Messenger::Loop(){}
