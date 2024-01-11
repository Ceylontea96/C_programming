#include <stdio.h>
#include "server.h"


void server_start(char* ip, char* port) {
	WSADATA wsaData; // 윈속 객체 생성
	SOCKET servSock, clintSock;
	SOCKADDR_IN servAddr, clintAddr;

	int szClntAddr;
	char message[100];
	int strLen;

	// ip port 변수 확인
	/*
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/
	

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // 윈속 객체 초기화
		ErrorHandling("WSAStartup() error!");
	}

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}
		

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port)); 

	if (bind(servSock, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind error()");
	}
		
	if (listen(servSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}
			
	szClntAddr = sizeof(clintAddr);
	clintSock = accept(servSock, (SOCKADDR*)& clintAddr, &szClntAddr);
	if (clintSock == INVALID_SOCKET) {
		ErrorHandling("accept() error");
	}
		
	while (1) {
		memset(message, '\0', sizeof(message));
		strLen = recv(clintSock, message, sizeof(message) - 1, 0);
		if (strLen == -1) {
			ErrorHandling("read() error!");
		}
				
		printf("Message from client : %s \n", message);
		printf("메시지 입력 >>");
		fgets(message, 100, stdin);
		send(clintSock, message, strlen(message) + 1, 0);
	}

	closesocket(clintSock);
	closesocket(servSock);
	WSACleanup();
}

