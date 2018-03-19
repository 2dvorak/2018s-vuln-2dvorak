#include "common.h"
#include "messenger.h"
using namespace std;
using namespace newmsger;

int main(int argc, char *argv[])
{
    if( argc != 2 ){
        cout << "Usage: ./onionMessenger [passphrase]" << endl;
    }
    else{
        Messenger *msger = new Messenger(argv[1]);
        msger->Loop();
    }
}
