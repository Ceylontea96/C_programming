#include "client.h";

void client_start(char* ip, char* port) {
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN addr = { 0 };

	char message[100];
	char temp[100];
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // 윈속 객체 초기화
		ErrorHandling("WSAStartup() error!");
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (connect(sock, (SOCKADDR*)& addr, sizeof(addr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	while (1) {
		printf("메시지 입력 >>");
		fgets(temp, 100, stdin);
		send(sock, temp, strlen(temp) + 1, 0);
		strLen = recv(sock, message, sizeof(message) - 1, 0);
		if (strLen <= 0) {
			ErrorHandling("read() error!");
		}
		printf("Message from server : %s \n", message);
	}

	closesocket(sock);
	WSACleanup();
}