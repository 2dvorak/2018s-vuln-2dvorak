#include "common.h"

// <githubID, *nodeInfo>
unordered_map<string, newkey::Nodeinfo*>* nodeMap;
unordered_map<string, newkey::Nodeinfo*>::iterator nodeIter;

// <githubID, chatList, newMsg, timestamp>
unordered_map<string, tuple<vector<string>*, unsigned int, time_t>*>* chatRoomMap;
unordered_map<string, tuple<vector<string>*, unsigned int, time_t>*>::iterator chatRoomIter;

queue<string>qSendMsg;
queue<string>qRecvMsg;
mutex r_mutex;
std::condition_variable r_cv;
mutex s_mutex;
mutex k_mutex;
string MyIP;
newkey::Keymanager *g_km;
PGPCrypt::PGPManager *PGP_m;

bool DEBUG = false;
bool DEMO = false;

void UsageMessenger(){
    cout << "Usage: ./onionMessenger [GithubID] [passPhrase]" << endl;
}

void CheckPW(){
    cout << "Check your password Plz..." << endl;
}

void CheckIP(){
    string ipAddress="Unable to get IP Address";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    success = getifaddrs(&interfaces);
    if (success == 0) {
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                if( (strcmp(temp_addr->ifa_name, "eth0")==0) || (strcmp(temp_addr->ifa_name, "docker0")==0) ){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(interfaces);
    MyIP = ipAddress;
}

bool RecvAvailable() {
    return get<0>(*(chatRoomIter->second))->size() > get<1>(*(chatRoomIter->second));
}

bool SendAvailable() {
    return !qSendMsg.empty();
}
