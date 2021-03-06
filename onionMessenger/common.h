#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <thread>
#include <unordered_map>
#include <queue>
#include <vector>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <termios.h>
#include <random>
#include <fstream>
#include <set>
#include <condition_variable>
#include <sys/time.h>
#include <list>

#include "json.hpp"
#include "keymanager.h"
#include "pgpmanager.h"

// message id macros
#define TYPE_KEY	0
#define TYPE_MSG	1
#define TYPE_QUERY	2

using namespace std;
using json = nlohmann::json;

extern unordered_map<string, newkey::Nodeinfo*>* nodeMap;
extern unordered_map<string, newkey::Nodeinfo*>::iterator nodeIter;

extern unordered_map<string, tuple<vector<string>*, unsigned int, time_t>*>* chatRoomMap;
extern unordered_map<string, tuple<vector<string>*, unsigned int, time_t>*>::iterator chatRoomIter;

// global queues
extern queue<string> qSendMsg;
extern queue<string> qRecvMsg;
extern string MyIP;
extern mutex s_mutex;
extern mutex r_mutex;
extern std::condition_variable r_cv;
extern mutex k_mutex;
extern newkey::Keymanager *g_km;
extern PGPCrypt::PGPManager *PGP_m;

extern bool DEBUG;
extern bool DEMO;

// Usage & Print stderr
void UsageMessenger();
void CheckPW();
void CheckIP();

// Thread wait
bool RecvAvailable();
bool SendAvailable();

#endif // COMMON_H
