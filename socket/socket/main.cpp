//#pragma comment(lib, "ws2_32");


#include "main.h"
#include "server.h"
#include "client.h"


int main(int argc, char *argv[]) { // 0:IP, 1:PORT, 2:Server or Client
	if (argc != 4) {
		printf("IP, PORT, Server �Ǵ� Client�� �Է����ֽʽÿ�.\n");
		printf("%s, %s, %s\n", argv[1], argv[2], argv[3]);
		exit(1);
	}

	// ������ ������ Ŭ���̾�Ʈ�� Ŭ���̾�Ʈ��
	server_start(argv[1], argv[2]);
	/*
	if (argv[3] == " Server")		server_start(argv[1], argv[2]);
	else if (argv[3] == " Client")	client_start(argv[1], argv[2]);
	else {
		printf("Server �Ǵ� Client�� �����ؾ��մϴ�.\n");
		printf("%s, %s, %s\n", argv[1], argv[2], argv[3]);
	}
	*/
	

	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}