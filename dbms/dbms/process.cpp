#include "process.h"

int SYSTEM_STATUS = LOGIN;

void loginMenu() {
	printf("\n============================= ���� �޴� =============================\n");
	printf("1. ȸ������, 2. �α���, 3. ����� ���, 4. ȸ��Ż��, 0. ���α׷� ����\n");
	char input[MAX] = { 0, };
	//char* input = getString();
	getString(input);

	if (strcmp(input, "0") == 0)		SYSTEM_STATUS = EXIT;
	else if (strcmp(input, "1") == 0)	sign_up();
	else if (strcmp(input, "2") == 0)	sign_in();
	else if (strcmp(input, "3") == 0)	showUsers();
	else if (strcmp(input, "4") == 0)	delete_account();
	else								printf("�ٽ� �Է����ּ���.\n");

	//free(input);
}

void sign_up() {	// ȸ������ (���ư���� -1�� ����)
	printf("\n========== ȸ�� ���� ========== (0. ���ư���)\n");
	char id[MAX] = { 0, }, pw[MAX] = { 0, };
	while (1) {
		printf("ID ");
		getString(id);
		if (strcmp(id, "0") == 0) return;
		if (getUserByName(id) == NULL) {
			printf("PW ");
			getString(pw);
			create_user(id, pw, 1);
			printf("ȸ������ ����!\n");
			break;
		}
		else printf("�ߺ��� ���̵��Դϴ�.\n");
	}
}

void sign_in() {	// �α��� (���ư���� -1�� ����)
	printf("\n========== �α��� ========== (0. ���ư���)\n");
	char id[MAX] = { 0, }, pw[MAX] = { 0, };
	while (1) {
		printf("ID ");
		getString(id);
		if (strcmp(id, "0") == 0) return;
		user * user = getUserByName(id);

		if (user != NULL) {
			while (1) {
				printf("PW ");
				getString(pw);
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
	char id[MAX] = { 0, }, pw[MAX] = { 0, };
	while (1) {
		printf("ID ");
		getString(id);
		if (strcmp(id, "0") == 0) return;
		user * user = getUserByName(id);

		if (user != NULL) {
			while (1) {
				printf("PW ");
				getString(pw);
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
	char query[MAX] = { 0, };
	if (getQuery(query) < 0) return;
	
	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);
	
	switch (checkDbCommand(command)) {
	case Create:
	{
		createDb(createParser(query));
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

void createDb(char* dbName) {
	database* db = getDbByName(dbName);
	if (db == NULL) {
		createDB(dbName);
		if (login_user->dlink == NULL) {
			login_user->dlink = dbTop;
		}
		printf("[%s] ���� ����.\n", dbName);
	}
	else	printf("�ߺ��� �����ͺ��̽����Դϴ�.\n");
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
	if (getDbByName(dbName) == NULL) printf("�������� �ʴ� �����ͺ��̽��Դϴ�[%s].\n", dbName);
	else {
		table* tb = dropDB(dbName);
		dropAllTb(tb);
		printf("[%s] �����ͺ��̽��� �����Ǿ����ϴ�.\n", dbName);
	}
}

void tableMenu() {
	printf("\n========== ���̺� ������ �Է��ϼ���. ==========\n");
	// 0.create, 1. show, 2. use, 3. drop, 4.select, 5. insert, 6. delete, 7. update, 0. logout
	char query[MAX] = { 0, };
	if (getQuery(query) < 0) return;

	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);

	switch (checkTbCommand(command)) {

	case Create:	//create table tb1(id varchar(100), pwd varchar(100), no int(10));
		tableCreateParser(query);
		break;
	case Show:	//show tables;
		showTables();
		break;
	case Use:	//use database db1;
		useDb(useParser(query));
		break;
	case Drop:	//drop table tb1;
		dropTB(tableNameParser(query));
		break;
	case Select:	//select * from tb1 where pwd = 'test';
		tableSelectParser(query);
		break;
	case Insert:	//insert into tb1(id, pwd, no) values('user', 'user1234', 1);
		tableInsertParser(query);
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

void tableCreateParser(char* query) {					//create table tb1(id varchar(100), pwd varchar(100), no int(10));	
	int result = 0;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);			//cmd[create] context[table tb1(id varchar(100), pwd varchar(100), no int(10))]
	if (_strcmpi(cmd, "create") == 0) {
		cmd = strtok_s(NULL, " ", &context);			//cmd[table] context[tb1(id varchar(100), pwd varchar(100), no int(10))]
		cmd = strtok_s(NULL, "(", &context);			//cmd[tb1] context[id varchar(100), pwd varchar(100), no int(10))]

		if (getTableByName(cmd) != NULL) {
			printf("�̹� �����ϴ� ���̺�\n");
			result = -1;
		}
		else {
			table* tb = createTable(cmd);
			//using_tb = tb;
			if (using_db->tlink == NULL) using_db->tlink = tableTop;

			cmd = strtok_s(NULL, ",", &context);		//cmd[id varchar(100)] context[ pwd varchar(100), no int(10))]
			while (cmd != NULL) {
				char* name = NULL, * type = NULL, * temp = NULL;
				char* last = context;
				int size = 0;

				name = strtok_s(cmd, " ", &temp);
				type = strtok_s(NULL, "(", &temp);
				size = atoi(strtok_s(NULL, " ", &temp));
				if (name == NULL || type == NULL || size == NULL) {
					//printf("�Ľ� ����\n");
					result = -1;
					break;
				}
				//printf("�÷� ����[%s][%s][%d]\n", name, type, size);
				columnTop = tb->clink;
				column* cl = createColumn(name, type, size);
				if (tb->clink == NULL) tb->clink = columnTop;
				last = context;
				cmd = strtok_s(last, ",", &context);		//cmd[id varchar(100)] context[ pwd varchar(100), no int(10))]
			}

			if (result == -1) {
				printf("���̺� ���� ����!\n");
				dropTable(tb);
			}
			else {
				printf("���̺� ���� ����![%s]\n", tb->tbname);

			}
		}
	}
}

void tableInsertParser(char* query) {
	int result = -1;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);			//cmd[insert] context[into tb1(id, pwd, no) values('user', 'user1234', 1);]
	if (_strcmpi(cmd, "insert") == 0) {
		cmd = strtok_s(NULL, " ", &context);				//cmd[into] context[tb1(id, pwd, no) values('user', 'user1234', 1);]
		if (_strcmpi(cmd, "into") == 0) {
			char* targets = NULL, * tbName = NULL, * values = NULL;
			tbName = strtok_s(NULL, "(", &context);				//cmd[tb1] context[id, pwd, no) values('user', 'user1234', 1);]
			table* tb = getTableByName(tbName);
			if (tb != NULL) {
				targets = strtok_s(NULL, ")", &context);				//cmd[id, pwd, no] context[ values('user', 'user1234', 1);]
				cmd = strtok_s(NULL, "(", &context);				//cmd[ values] context['user', 'user1234', 1);]
				if (_strcmpi(cmd, " values") == 0) {
					values = strtok_s(NULL, ")", &context);				//cmd['user', 'user1234', 1] context[]
					//printf("targets[%s], values[%s]\n", targets, values);
					result = InsertParser(tb, targets, values);
				}
			}
		}
	}
	if (result < 0) printf("�߸��� �����Դϴ�.\n");
	else printf("Insert ����\n");
}

int InsertParser(table* tb, char* targets, char* values) { //targets[id, pwd, no] values['user', 'user1234', 1]
	int MaxIdx, suc_flag = 1;
	char* context1 = NULL, *context2 = NULL, *target = NULL, *value = NULL;
	column* cl = tb->clink;
	MaxIdx = getNumberOfDatas(cl);
	target = strtok_s(targets, ",", &context1);
	value = strtok_s(values, ",", &context2);

	while (target != NULL && value != NULL) {
		column* cl = tb->clink;
		bool flag = false;
		while (cl != NULL) {
			if (_strcmpi(cl->field, trim(target)) == 0) {
				flag = true;
				int re = createData(cl, trim(value));
				if (re < 0) {
					suc_flag = -1;
					break;
				}
			}
			cl = cl->link;
		}
		if (!flag) {
			suc_flag = -1;
			printf("column %s�� �������� ����\n", target);
		}
		if (suc_flag == -1) {
			break;
		}

		targets = context1;
		values = context2;
		target = strtok_s(targets, ",", &context1);
		value = strtok_s(values, ",", &context2);
	}

	if (suc_flag == -1) {	// insert ���� ���ܹ߻� �� 
		printf("insert ����\n");
		cl = tb->clink;
		while (cl != NULL) {
			data* preData = NULL;
;			data* dt = cl->dlink;
			while (dt != NULL) {
				preData = dt;
				if (dt->index == MaxIdx) {
					dropData(cl, dt);
					break;
				}
				dt = dt->link;
			}
			cl = cl->link;
		}
	}
	else {		// insert���� ���� ������ �����͸� null�� ä�� �߰����ִ� �κ�
		cl = tb->clink;
		while (cl != NULL) {
			int index = getNumberOfDatas(cl);
			if (index == MaxIdx) createData(cl, NULL);
			cl = cl->link;
		}
	}

	return suc_flag;
}

void tableSelectParser(char* query) {	//select id,pwd from tb1;
	int result = -1;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);			//cmd[select] context[id,pwd from tb1]
	char *tbName = NULL, *target = NULL, *option = NULL;
	if (_strcmpi(cmd, "select") == 0) {
		target = strtok_s(NULL, " ", &context);			//target[*] context[from tb1]
		cmd = strtok_s(NULL, " ", &context);			//cmd[from] context[tb1]
		if (_strcmpi(cmd, "from") == 0) {
			tbName = strtok_s(NULL, " ", &context);		//tbName[tb1] context[]
			if (tbName != NULL && (context == NULL || context[0] == '\0')) {
				selectTable(tbName, target, NULL);
				result = 0;
			}
			else {
				cmd = strtok_s(NULL, " ", &context);	//tbName[where] context[no = 1 and pwd = 'sfd.34']
				if (_strcmpi(cmd, "where") == 0) {
					printf("select test [%s] [%s]\n", cmd, context);
					selectTable(tbName, target, context);
				}
			}
		}
	}
	if (result == -1) printf("SELECT ����\n");
}

void tableDeleteParser(char* query) {
	int result = -1;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);		//cmd[delete] context[from tb1 where no = 1]
	if (_strcmpi(cmd, "delete") == 0) {
		cmd = strtok_s(NULL, " ", &context);		//cmd[from] context[tb1 where no = 1]
		if (_strcmpi(cmd, "from") == 0) {
			char* tbName = strtok_s(NULL, " ", &context);		//cmd[tb1] context[where no = 1]
			
			if (tbName != NULL) {
				if(context == NULL) result = deleteTb(tbName, NULL);
				else				result = deleteTb(tbName, context);
			}
			
		}
		
	}
	if (result == -1) printf("�߸��� ����\n");
	else printf("delete ����\n");
}

void dropTB(char* tbName) {
	if (getTableByName(tbName) == NULL) printf("�������� �ʴ� ���̺��Դϴ�[%s].\n", tbName);
	else {
		column* column = dropTable(tbName);
		dropAllColumn(column);
		printf("[%s] ���̺��� �����Ǿ����ϴ�.\n", tbName);
	}
}

void dropAllDb(database* databaseHead) {
	database* targetDb = databaseHead;
	database* nextDb = NULL;
	while (targetDb != NULL) {
		nextDb = targetDb->link;
		dropAllTb(targetDb->tlink);
		free(targetDb);
		targetDb = nextDb;
	}
}

void dropAllTb(table* tableHead) {
	table* targetTable = tableHead;
	table* nextTable = NULL;
	while (targetTable != NULL) {
		nextTable = targetTable->link;
		dropAllColumn(targetTable->clink);
		free(targetTable);
		targetTable = nextTable;
	}
}

void dropAllColumn(column* columnHead) {
	column* targetColumn = columnHead;
	column* nextCol = NULL;
	while (targetColumn != NULL) {
		nextCol = targetColumn->link;
		dropAllData(targetColumn->dlink);
		free(targetColumn);
		targetColumn = nextCol;
	}
}

void dropAllData(data* dataHead) {
	data* target_data = dataHead;
	data* nextData = NULL;
	while (target_data != NULL) {
		nextData = target_data->link;
		free(target_data);
		target_data = nextData;
	}
}



