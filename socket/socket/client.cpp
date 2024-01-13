#include "client.h";

extern char* IP;
extern char* PORT;
extern char* NAME;

void client_start() {
	SOCKET servSock;
	SOCKADDR_IN servAddr = { 0 };
	HANDLE sendThread, recvThread;

	servSock = clinet_init();
	
	sendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&servSock, 0, NULL);
	recvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&servSock, 0, NULL);

	WaitForSingleObject(sendThread, INFINITE);
	WaitForSingleObject(recvThread, INFINITE);

	closesocket(servSock);
	WSACleanup();
}

int clinet_init() {
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr = { 0 };

	printf("### CLIENT INIT <%s> : <%s> ###\n", IP, PORT);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // 윈속 객체 초기화
		ErrorHandling("WSAStartup() error!");
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	if (inet_pton(AF_INET, IP, &(servAddr.sin_addr)) == -1) ErrorHandling("inet_pton error()");
	servAddr.sin_port = htons(atoi(PORT));

	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}
	printf("연결 성공!\n");

	return sock;
}

unsigned WINAPI SendMsg(void* arg) {
	SOCKET sock = *((SOCKET*)arg);
	char msg[MSG_BUF];
	char nameMsg[NAME_SIZE + MSG_BUF];
	while (1) {
		fgets(msg, MSG_BUF, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
			closesocket(sock);
			exit(0);
		}
		sprintf_s(nameMsg, sizeof(nameMsg), "[%s] %s", NAME, msg);
		send(sock, nameMsg, strlen(nameMsg), 0);
	}
	return 0;
}

unsigned WINAPI RecvMsg(void* arg) {
	SOCKET sock = *((SOCKET*)arg);

	char nameMsg[NAME_SIZE + MSG_BUF];
	int strLen;
	while (1) {
		strLen = recv(sock, nameMsg, NAME_SIZE + MSG_BUF - 1, 0);
		if (strLen == -1) return -1;
		nameMsg[strLen] = 0; // 문자열의 끝을 알리기 위해 설정
		fputs(nameMsg, stdout);
	}
	return 0;
}