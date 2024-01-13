#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <process.h>

#define MSG_BUF 100
#define NAME_SIZE 15

void ErrorHandling(const char* message);
