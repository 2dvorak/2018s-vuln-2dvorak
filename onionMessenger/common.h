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

#include "json.hpp"

using namespace std;

// global queues
extern queue<string> qkey;
extern queue<string> qSendMsg;
extern queue<string> qRecvMsg;

// Usage & Print stderr
void UsageMessenger();
void CheckPW();

#endif // COMMON_H
