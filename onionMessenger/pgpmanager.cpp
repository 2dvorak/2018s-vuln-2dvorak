#include "pgpmanager.h"
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

namespace PGPCrypt{

    PGPManager::PGPManager(){}

    PGPManager::~PGPManager(){}

    string PGPManager::Enc(string input_plain){}

    string PGPManager::Dec(string input_cipher){}
}
