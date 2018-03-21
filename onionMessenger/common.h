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

using namespace std;

// Usage & Print stderr
void UsageMessenger();
void CheckPW();

#endif // COMMON_H
