#include "process.h"

int SYSTEM_STATUS = LOGIN;

void loginMenu() {
	printf("\n============================= ���� �޴� =============================\n");
	printf("1. ȸ������, 2. �α���, 3. ����� ���, 4. ȸ��Ż��, 0. ���α׷� ����\n");
	char* input = getString();

	if (strcmp(input, "0") == 0)		SYSTEM_STATUS = EXIT;
	else if (strcmp(input, "1") == 0)	sign_up();
	else if (strcmp(input, "2") == 0)	sign_in();
	else if (strcmp(input, "3") == 0)	showUsers();
	else if (strcmp(input, "4") == 0)	delete_account();
	else								printf("�ٽ� �Է����ּ���.\n");
}

void sign_up() {	// ȸ������ (���ư���� -1�� ����)
	printf("\n========== ȸ�� ���� ========== (0. ���ư���)\n");
	char* id, * pw;
	while (1) {
		printf("ID ");
		id = getString();
		if (strcmp(id, "0") == 0) return;
		if (getUserByName(id) == NULL) {
			printf("PW ");
			pw = getString();
			create_user(id, pw, 1);
			printf("ȸ������ ����!\n");
			break;
		}
		else printf("�ߺ��� ���̵��Դϴ�.\n");
	}
}

void sign_in() {	// �α��� (���ư���� -1�� ����)
	printf("\n========== �α��� ========== (0. ���ư���)\n");
	char *id, *pw;
	while (1) {
		
		printf("ID ");
		id = getString();
		if (strcmp(id, "0") == 0) return;
		user * user = getUserByName(id);

		if (user != NULL) {
			while (1) {
				printf("PW ");
				pw = getString();
				if (strcmp(pw, "0") == 0) return;
				if (checkPw(id, pw)) {
					login_user = user;
					dbTop = login_user->dlink;
					printf("�α��� ����!\n");
					SYSTEM_STATUS = DATABASE;
					return;
				} else printf("�������� ��й�ȣ�Դϴ�.\n");
			}
		}
		else printf("�������� �ʴ� ���̵��Դϴ�.\n");
	}
}

void delete_account() {
	printf("\n========== ȸ�� Ż�� ========== (0. ���ư���)\n");
	char* id, * pw;
	while (1) {
		printf("ID ");
		id = getString();
		if (strcmp(id, "0") == 0) return;
		user * user = getUserByName(id);

		if (user != NULL) {
			while (1) {
				printf("PW ");
				pw = getString();
				if (strcmp(pw, "0") == 0) return;
				if (checkPw(id, pw)) {
					delete_user(id);
					printf("ȸ�� Ż��Ǿ����ϴ�.\n");
					return;
				}
				else printf("�������� ��й�ȣ�Դϴ�.\n");
			}
		}
		else printf("�������� �ʴ� ���̵��Դϴ�.\n");
	}
}


void DBMenu() {
	printf("\n========== �����ͺ��̽� ������ �Է��ϼ���. ==========\n");
	// 1.create, 2. show, 3. use, 4. drop, 0. logout
	printf("���� ");
	char* query = getString();
	if (_strcmpi(query, "logout") == 0) {
		login_user = NULL;
		SYSTEM_STATUS = LOGIN;
		return;
	}
	/*
	printf("strtok �� : [%s]\n", query);
	char* temp = NULL;
	char* str = strtok_s(query, " ", &temp);
	printf("strtok 1ȸ : [%s][%s]\n", query, temp);
	str = strtok_s(query, " ", &temp);
	printf("strtok 2ȸ : [%s][%s]\n", query, temp);
	*/
	
	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);
	int cmd = checkCommand(command);
	
	if (cmd == Error) printf("�߸��� �����Դϴ�.\n");
	else if (cmd == Create) {
		printf("test1\n");
		char* dbName = createParser(query);
		printf("test11\n");
		if (getDbByName(dbName) == NULL)	createDB(dbName);
		else								printf("�ߺ��� �����ͺ��̽����Դϴ�.\n");
		
	} else if (cmd == Show) {

	}
	else if (cmd == Use) {
		printf("test\n");
		char* dbName = useParser(query);
		printf("test10\n");
		database* db = getDbByName(dbName);
		printf("test20\n");
		if (db == NULL) printf("�������� �ʴ� �����ͺ��̽��Դϴ�.\n");
		else {
			printf("test30\n");
			using_db = db;
			tableTop = using_db->tlink;
			SYSTEM_STATUS = TABLE;
			printf("[%s] �����ͺ��̽� ���� ����!\n", dbName);
		}
	}
	else if (cmd == Drop) {

	}
	
	
	/*
	if (strcmp(input, "0") == 0)		SYSTEM_STATUS = EXIT;
	else if (strcmp(input, "1") == 0)	sign_up();
	else if (strcmp(input, "2") == 0)	sign_in();
	else if (strcmp(input, "3") == 0)	showUsers();
	else if (strcmp(input, "4") == 0)	delete_account();
	else								printf("�ٽ� �Է����ּ���.\n");
	while (1) {
		printf("\n========== �����ͺ��̽� ������ �Է��ϼ���. ==========\n");
		char* input = getString();
		
		char command[MAX] = { 0 };
		sprintf_s(command, MAX, "%s", input);
		int option = dbParsing(command);
		if (option == -1) printf("�߸��� �����Դϴ�\n");

		char* temp = NULL;
		char* cmd = strtok_s(input, " ", &temp);
		if (option == 1) {	//create
			cmd = strtok_s(NULL, " ", &temp);
			if (createDB(temp) == 0) printf("�����ͺ��̽� ���� ����! [%s]\n", temp);
			else					 printf("�̹� �����ϴ� �����ͺ��̽��Դϴ�.[%s]\n", temp);
		}
		if (option == 2) {	//use
			if (useDB(temp) ==  0) {
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
	*/

	
	return;
}

void dropAllTb(table* tb) {
	table* nextTb = tb->link;
	while (tb != NULL) {
		dropTB(tb);
		tb = nextTb;
	}
}



