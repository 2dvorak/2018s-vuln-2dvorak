
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

void Messenger::Loop(){}

}
