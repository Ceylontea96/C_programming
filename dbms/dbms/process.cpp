#include "process.h"

int SYSTEM_STATUS = LOGIN;

void loginMenu() {
	printf("\n============================= 메인 메뉴 =============================\n");
	printf("1. 회원가입, 2. 로그인, 3. 사용자 목록, 4. 회원탈퇴, 0. 프로그램 종료\n");
	char* input = getString();

	if (strcmp(input, "0") == 0)		SYSTEM_STATUS = EXIT;
	else if (strcmp(input, "1") == 0)	sign_up();
	else if (strcmp(input, "2") == 0)	sign_in();
	else if (strcmp(input, "3") == 0)	showUsers();
	else if (strcmp(input, "4") == 0)	delete_account();
	else								printf("다시 입력해주세요.\n");
}

void sign_up() {	// 회원가입 (돌아가기면 -1을 리턴)
	printf("\n========== 회원 가입 ========== (0. 돌아가기)\n");
	char* id, * pw;
	while (1) {
		printf("ID ");
		id = getString();
		if (strcmp(id, "0") == 0) return;
		if (getUserByName(id) == NULL) {
			printf("PW ");
			pw = getString();
			create_user(id, pw, 1);
			printf("회원가입 성공!\n");
			break;
		}
		else printf("중복된 아이디입니다.\n");
	}
}

void sign_in() {	// 로그인 (돌아가기면 -1을 리턴)
	printf("\n========== 로그인 ========== (0. 돌아가기)\n");
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
					printf("로그인 성공!\n");
					SYSTEM_STATUS = DATABASE;
					return;
				} else printf("옳지않은 비밀번호입니다.\n");
			}
		}
		else printf("존재하지 않는 아이디입니다.\n");
	}
}

void delete_account() {
	printf("\n========== 회원 탈퇴 ========== (0. 돌아가기)\n");
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
					printf("회원 탈퇴되었습니다.\n");
					return;
				}
				else printf("옳지않은 비밀번호입니다.\n");
			}
		}
		else printf("존재하지 않는 아이디입니다.\n");
	}
}


void DBMenu() {
	printf("\n========== 데이터베이스 쿼리를 입력하세요. ==========\n");
	// 0.create, 1. show, 2. use, 3. drop, 0. logout
	printf("쿼리 ");
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
			printf("[%s] 생성 성공.\n", dbName);
		}
		else	printf("중복된 데이터베이스명입니다.\n");
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
		printf("잘못된 쿼리입니다.\n");
		break;
	}
	
	return;
}


void useDb(char* dbName) {
	database* db = getDbByName(dbName);
	if (db == NULL) printf("존재하지 않는 데이터베이스입니다.\n");
	else {
		using_db = db;
		tableTop = using_db->tlink;
		SYSTEM_STATUS = TABLE;
		printf("[%s] 데이터베이스 접속 성공!\n", dbName);
	}
}

void dropDb(char* dbName) {
	if (getDbByName(dbName) == NULL) printf("존재하지 않는 데이터베이스입니다.\n");
	else {
		table* tb = dropDB(dbName);
		printf("[%s] DB가 삭제되었습니다.\n", dbName);
	}
	
	
	//dropAllTb();
}

void tableMenu() {
	printf("\n========== 테이블 쿼리를 입력하세요. ==========\n");
	// 0.create, 1. show, 2. use, 3. drop, 4.select, 5. insert, 6. delete, 7. update, 0. logout
	printf("쿼리 ");
	char* query = getQuery();

	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);

	switch (checkCommand(command)) {
	case Create:
	{
		
		break;
	}
	case Show:
		showTables();
		break;
	case Use:
		useDb(useParser(query));
		break;
	case Drop:
		
		break;
	case Select:

		break;
	case Insert:

		break;
	case Delete:

		break;
	case Update:

		break;
	case Logout:
		login_user = NULL;
		using_db = NULL;
		SYSTEM_STATUS = LOGIN;
		break;
	default:
		printf("잘못된 쿼리입니다.\n");
		break;
	}

	return;
}




void createTb(char* query) {
	

	tableInfoParser();

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
