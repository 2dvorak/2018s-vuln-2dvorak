#include "common.h"
#include "messenger.h"
using namespace newmsger;

int main(int argc, char *argv[])
{
    if( argc != 2 ){
        UsageMessenger();
    }
    else{
        Messenger *msger = new Messenger(argv[1]);
        msger->Loop();
    }
}
