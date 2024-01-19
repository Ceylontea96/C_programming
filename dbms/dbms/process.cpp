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
					database* db = delete_user(id);
					dropAllDb(db);
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
	// 0.create, 1. show, 2. use, 3. drop, 0. logout
	printf("���� ");
	char* query = getQuery();
	
	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);
	
	switch (checkCommand(command)) {
	case Create:
	{
		char* dbName = createParser(query);
		database* db = getDbByName(dbName);
		if (db == NULL) {
			if (login_user->dlink == NULL) {
				login_user->dlink = dbTop;
			}
			createDB(dbName);
			printf("[%s] ���� ����.\n", dbName);
		}
		else	printf("�ߺ��� �����ͺ��̽����Դϴ�.\n");
		break;
	}
	case Show:
		showDbs();
		break;
	case Use:
		useDb(useParser(query));
		break;
	case Drop:
		dropDb(dropParser(query));
		break;
	case Logout:
		login_user = NULL;
		SYSTEM_STATUS = LOGIN;
		break;
	default:
		printf("�߸��� �����Դϴ�.\n");
		break;
	}
	
	return;
}

void useDb(char* dbName) {
	database* db = getDbByName(dbName);
	if (db == NULL) printf("�������� �ʴ� �����ͺ��̽��Դϴ�.\n");
	else {
		using_db = db;
		tableTop = using_db->tlink;
		SYSTEM_STATUS = TABLE;
		printf("[%s] �����ͺ��̽� ���� ����!\n", dbName);
	}
}

void dropDb(char* dbName) {
	if (getDbByName(dbName) == NULL) printf("�������� �ʴ� �����ͺ��̽��Դϴ�.\n");
	else {
		table* tb = dropDB(dbName);
		dropAllTb(tb);
		printf("[%s] �����ͺ��̽��� �����Ǿ����ϴ�.\n", dbName);
	}
	
	
	//dropAllTb();
}

void tableMenu() {
	printf("\n========== ���̺� ������ �Է��ϼ���. ==========\n");
	// 0.create, 1. show, 2. use, 3. drop, 4.select, 5. insert, 6. delete, 7. update, 0. logout
	printf("���� ");
	char* query = getQuery();

	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);

	switch (checkCommand(command)) {

	case Create:	//create table tb1(id varchar(100), pwd varchar(100), no int(10));
	{
		printf("create!!\n");
		break;
	}
	case Show:	//show tables;
		printf("show!!\n");
		showTables();
		break;
	case Use:	//use database db1;
		printf("use!!\n");
		useDb(useParser(query));
		break;
	case Drop:	//drop table tb1;
		dropTB(dropParser(query));
		printf("drop!!\n");
		break;
	case Select:	//select * from tb1 where pwd = 'test';
		printf("select!!\n");
		break;
	case Insert:	//insert into tb1(id, pwd, no) values('user', 'user1234', 1);
		printf("insert!!\n");
		break;
	case Delete:	//delete from tb1 where no = 1;
		printf("delete!!\n");
		break;
	case Update:	//update tb1 set id='test' where pwd = 'test';
		printf("update!!\n");
		break;
	case Logout:
		login_user = NULL;
		using_db = NULL;
		SYSTEM_STATUS = LOGIN;
		break;
	default:
		printf("�߸��� �����Դϴ�.\n");
		break;
	}

	return;
}




void createTb(char* query) {
	

	tableInfoParser(query);

}

void dropTB(char* tbName) {
	if (getTableByName(tbName) == NULL) printf("�������� �ʴ� ���̺��Դϴ�.\n");
	else {
		column* column = dropTable(tbName);
		dropAllColumn(column);
		printf("[%s] ���̺��� �����Ǿ����ϴ�.\n", tbName);
	}
}

void dropAllDb(database* db) {
	database* nextDb = NULL;
	while (db != NULL) {
		nextDb = db->link;
		dropAllTb(db->tlink);
		free(db);
		db = nextDb;
	}
}

void dropAllTb(table* tb) {
	table* nextTb = tb;
	while (tb != NULL) {
		nextTb = tb->link;
		dropAllColumn(tb->clink);
		dropTable(tb);
		tb = nextTb;
	}
}

void dropAllColumn(column* col) {
	column* nextCol = NULL;
	while (col != NULL) {
		nextCol = col->link;
		dropAllData(col->dlink);
		free(col);
		col = nextCol;
	}
}

void dropAllData(data* data) {
	/*
	data* dt = NULL;
	while (data != NULL) {
		dt = data->link;
		free(data);
		data = dt;
	}
	*/
}
