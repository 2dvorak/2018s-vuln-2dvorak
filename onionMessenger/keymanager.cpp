
#include "common.h"
#include "keymanager.h"

namespace newkey{

    Keymanager::Keymanager(string githubID, string passPhrase)
    {
        this->passPhrase = passPhrase;
        this->githubID = githubID;
        this->nodeMap = new unordered_map<string, Nodeinfo*>;
    }

    Keymanager::~Keymanager(){}

    bool Keymanager::Validation(){return 0;}

    void Keymanager::SearchMap(string githubID){}

    void Keymanager::AddMap(string githubID){}

    void Keymanager::DelMap(string githubID){}

    void Keymanager::GetKey(){}

    void Keymanager::PutKey(){}

}
