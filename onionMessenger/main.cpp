#include "common.h"
#include "message.h"
#include "newproto.h"
#include "pgpcrypto.h"
#include "keymanager.h"

using namespace std;
using namespace newmsg;
using namespace newpro;
using namespace newcrypto;
using namespace newkey;

int main(int argc, char *argv[])
{
    string test;
    if( argc != 2 ){
        cout << "Usage: ./onionMessenger [Private_Key]" << endl;
    }
    else{
        cout << "Enter passphrase: ";
        cin >> test;

        cout << argv[1] << endl;
        cout << test << endl;
    }

    return 0;
}
