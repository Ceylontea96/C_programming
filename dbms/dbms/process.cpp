#include "process.h"

int loginMenu() {	// (���α׷� ����� -1�� ����)
	while (1) {
		printf("1. ȸ������, 2. �α���, 3. ����� ���, 4. ȸ��Ż��, 0. ���α׷� ����\n");
		char* input = getString();
		if (strcmp(input, "0") == 0) return -1;
		else if (strcmp(input, "1") == 0) {
			sign_up();
		}
		else if (strcmp(input, "2") == 0) {
			//int result = sign_in();
			if (sign_in() > -1) break;
		}
		else if (strcmp(input, "3") == 0) {
			showUsers();
		}
		else if (strcmp(input, "4") == 0) {
			//ȸ��Ż��
		}
		else {
			printf("[%s]\n", input);
		}
	}
	return 0;
}

int sign_in() {	// �α��� (���ư���� -1�� ����)
	char* id = NULL;
	while (1) {
		printf("\n========== �α��� ========== (0. ���ư���)\n");
		printf("ID : ");
		id = getString();
		if (strcmp(id, "0") == 0) return -1;
		if (isDupId(id)) {
			printf("PW : ");
			char* pw = getString();
			if (strcmp(pw, "0") == 0) return -1;
			if (checkPw(id, pw)) break;
		}
		else printf("�������� �ʴ� ���̵��Դϴ�\n");
	}
	printf("�α��� ���� [%s]\n", id);
	return 0;
}

void sign_up() {	// ȸ������ (���ư���� -1�� ����)
	while (1) {
		printf("\n========== ȸ�� ���� ========== (0. ���ư���)\n");
		printf("ID : ");
		char* id = getString();
		if (strcmp(id, "0") == 0) break;
		if (!isDupId(id)) {
			printf("PW : ");
			char* pw = getString();
			if (strcmp(pw, "0") == 0) break;
			create_user(id, pw, 1);
			printf("ȸ�� ���� ���� [%s]\n", id);
			break;
		}
	}
}

int DBMenu() { // (�α׾ƿ��̸� -1�� ����)
	while (1) {
		printf("\n========== �����ͺ��̽� ������ �Է��ϼ���. ==========\n");
		char* input = getString();
		// 1.create, 2. use, 3. show, 4. drop, 0. logout
		int option = dbParsing(input);
		if (option == -1) printf("�߸��� �����Դϴ�\n");
		char* temp = NULL;
		char* cmd = strtok_s(NULL, " ", &temp);
		if (option == 1) {	//create
			char* cmd = strtok_s(NULL, " ", &temp);
			if (createDB(temp)) printf("�����ͺ��̽� ���� ����! [%s]\n", temp);
			else				printf("�̹� �����ϴ� �����ͺ��̽��Դϴ�.\n");
		}
		if (option == 2) {	//use
			if (useDB(temp)) {
				printf("�����ͺ��̽� ���� ����! [%s]\n", temp);
				break;
			}
			else printf("�������� �ʴ� �����ͺ��̽��Դϴ�.\n");
		}
		if (option == 3) {	//show
			showDbs();
		}
		if (option == 4) {	//drop
			table* table = dropDB(temp);
			if(table == NULL)	printf("�������� �ʴ� �����ͺ��̽��Դϴ�.\n");
			else				dropAllTb(table);
		}
		if (option == 0) {
			printf("�α׾ƿ��մϴ�\n");
			return -1;
		}
	}
	return 0;
}

void dropAllTb(table* tb) {
	table* nextTb = tb->link;
	while (tb != NULL) {
		dropTB(tb);
		tb = nextTb;
	}
}



