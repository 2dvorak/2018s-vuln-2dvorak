#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <thread>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ncurses.h>

#include "json.hpp"

// message id macros
#define TYPE_KEY	0
#define TYPE_MSG	1
#define TYPE_QUERY	2

using namespace std;
using json = nlohmann::json;

// global queues
extern queue<string> qkey;
extern queue<string> qSendMsg;
extern queue<string> qRecvMsg;

// Usage & Print stderr
void UsageMessenger();
void CheckPW();

#endif // COMMON_H
