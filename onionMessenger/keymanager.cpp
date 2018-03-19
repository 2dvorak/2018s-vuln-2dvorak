
#include "common.h"
#include "keymanager.h"

using namespace newkey;

Keymanager::Keymanager(string pP)
{
    this->passPhrase = pP;
    this->nodeMap = new unordered_map<string, string>;
}

Keymanager::~Keymanager(){}

bool Keymanager::Validation(){}


