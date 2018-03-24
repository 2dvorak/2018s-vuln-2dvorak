#include "common.h"

// <githubID, *nodeInfo>
unordered_map<string, newkey::Nodeinfo*>* nodeMap;
unordered_map<string, newkey::Nodeinfo*>::iterator nodeIter;

queue<string>qSendMsg;
queue<string>qRecvMsg;
mutex r_mutex;
mutex s_mutex;
string MyIP;
newkey::Keymanager *g_km;

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
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if(strcmp(temp_addr->ifa_name, "docker0")==0){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    MyIP = ipAddress;
}
