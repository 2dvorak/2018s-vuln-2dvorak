
#include "common.h"
#include "messenger.h"
#include "keymanager.h"

using namespace std;
using namespace newkey;
using namespace newmsger;

Messenger::Messenger(string passPhrase)
{
    this->km = new Keymanager(passPhrase);

}

Messenger::~Messenger(){}

void Messenger::Loop(){}
