#pragma comment(lib, "ws2_32")
#include "main.h"
#include "server.h"
#include "client.h"

char* IP;
char* PORT;
char* NAME;

int main(int argc, char *argv[]) { // 0:IP, 1:PORT, 2:Server or Client
	if (argc != 4) {
		printf("IP, PORT, Server 또는 닉네임을 입력해주십시오.\n");
		exit(1);
	}
	
	IP = argv[1];
	PORT = argv[2];
	NAME = argv[3];
	
	if (strcmp(NAME, "Server") == 0)	server_start();
	else 								client_start();
	
	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}