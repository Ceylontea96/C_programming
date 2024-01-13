#include "server.h"

extern char* IP;
extern char* PORT;
extern char* NAME;

int clientCount = 0;
HANDLE Mutex;
SOCKET clientSocks[100];

void server_start() {
	SOCKET servSock, clientSock;
	SOCKADDR_IN clientAddr;
	HANDLE s_thread, c_thread;

	int szClntAddr;
	
	servSock = server_init();


	while (1) {
		szClntAddr = sizeof(clientAddr);
		clientSock = accept(servSock, (SOCKADDR*)&clientAddr, &szClntAddr);
		if (clientSock == INVALID_SOCKET) {
			ErrorHandling("accept() error");
		}
		else {
			printf("연결 중!\n");
		}
		WaitForSingleObject(Mutex, INFINITE);// 뮤텍스 실행
		clientSocks[clientCount++] = clientSock;
		ReleaseMutex(Mutex);
		s_thread = (HANDLE)_beginthreadex(NULL, 0, HandleServer, (void*)&clientSock, 0, NULL);
		c_thread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&clientSock, 0, NULL);
		char ip_addr[32] = { 0 };
		printf("Connected Client IP : %s\n", inet_ntop(AF_INET, &clientAddr.sin_addr, ip_addr, sizeof(ip_addr)));
	}
	
	closesocket(servSock);
	WSACleanup();
}

int server_init() {
	WSADATA wsaData; // 윈속 객체 생성
	SOCKET sock;
	SOCKADDR_IN servAddr;

	printf("### SERVER INIT <%s> : <%s> ###\n", IP, PORT);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // 윈속 객체 초기화
		ErrorHandling("WSAStartup() error!");
	}

	Mutex = CreateMutex(NULL, FALSE, NULL);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	if (inet_pton(AF_INET, IP, &(servAddr.sin_addr)) == -1) ErrorHandling("inet_pton error()");
	servAddr.sin_port = htons(atoi(PORT));

	if (bind(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind error()");
	}

	if (listen(sock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	return sock;
}

unsigned WINAPI HandleServer(void* arg) {
	SOCKET sock = *((SOCKET*)arg);
	char msg[MSG_BUF];
	char nameMsg[NAME_SIZE + MSG_BUF];
	while (1) {
		fgets(msg, MSG_BUF, stdin);

		sprintf_s(nameMsg, sizeof(nameMsg), "[SERVER] %s", msg);
		send(sock, nameMsg, strlen(nameMsg), 0);
	}
	return 0;
}

unsigned WINAPI HandleClient(void* arg) {
	SOCKET sock = *((SOCKET*)arg);
	int strLen = 0;
	char msg[MSG_BUF];

	while ((strLen = recv(sock, msg, sizeof(msg), 0)) != 0) {
		SendMsg(msg, strLen);
	}

	WaitForSingleObject(Mutex, INFINITE);
	for (int i = 0; i < clientCount; ++i) {
		if (sock == clientSocks[i]) {
			while (i++ < clientCount - 1) {
				clientSocks[i] = clientSocks[i + 1];
			}
			break;
		}
		--clientCount;
	}
	//printf("Connected Client IP : %s\n", inet_ntop(AF_INET, &clientAddr.sin_addr, ip_addr, sizeof(ip_addr)));
	ReleaseMutex(Mutex);
	closesocket(sock);
	return 0;
}

void SendMsg(char* msg, int len) {
	WaitForSingleObject(Mutex, INFINITE);
	for (int i = 0; i < clientCount; ++i) {
		send(clientSocks[i], msg, len, 0);
	}
	ReleaseMutex(Mutex);
}

