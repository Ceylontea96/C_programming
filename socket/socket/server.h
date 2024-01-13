#include "main.h"


void server_start();
int server_init();
unsigned WINAPI HandleServer(void* arg);
unsigned WINAPI HandleClient(void* arg);
void SendMsg(char* msg, int len);
