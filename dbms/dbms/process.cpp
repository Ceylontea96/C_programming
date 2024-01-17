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
					delete_user(id);
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
	// 1.create, 2. show, 3. use, 4. drop, 0. logout
	printf("쿼리 ");
	char* query = getString();
	if (_strcmpi(query, "logout") == 0) {
		login_user = NULL;
		SYSTEM_STATUS = LOGIN;
		return;
	}
	/*
	printf("strtok 전 : [%s]\n", query);
	char* temp = NULL;
	char* str = strtok_s(query, " ", &temp);
	printf("strtok 1회 : [%s][%s]\n", query, temp);
	str = strtok_s(query, " ", &temp);
	printf("strtok 2회 : [%s][%s]\n", query, temp);
	*/
	
	char command[MAX] = { 0 };
	sprintf_s(command, MAX, "%s", query);
	int cmd = checkCommand(command);
	
	if (cmd == Error) printf("잘못된 쿼리입니다.\n");
	else if (cmd == Create) {
		printf("test1\n");
		char* dbName = createParser(query);
		printf("test11\n");
		if (getDbByName(dbName) == NULL)	createDB(dbName);
		else								printf("중복된 데이터베이스명입니다.\n");
		
	} else if (cmd == Show) {

	}
	else if (cmd == Use) {
		printf("test\n");
		char* dbName = useParser(query);
		printf("test10\n");
		database* db = getDbByName(dbName);
		printf("test20\n");
		if (db == NULL) printf("존재하지 않는 데이터베이스입니다.\n");
		else {
			printf("test30\n");
			using_db = db;
			tableTop = using_db->tlink;
			SYSTEM_STATUS = TABLE;
			printf("[%s] 데이터베이스 접속 성공!\n", dbName);
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
	else								printf("다시 입력해주세요.\n");
	while (1) {
		printf("\n========== 데이터베이스 쿼리를 입력하세요. ==========\n");
		char* input = getString();
		
		char command[MAX] = { 0 };
		sprintf_s(command, MAX, "%s", input);
		int option = dbParsing(command);
		if (option == -1) printf("잘못된 쿼리입니다\n");

		char* temp = NULL;
		char* cmd = strtok_s(input, " ", &temp);
		if (option == 1) {	//create
			cmd = strtok_s(NULL, " ", &temp);
			if (createDB(temp) == 0) printf("데이터베이스 생성 성공! [%s]\n", temp);
			else					 printf("이미 존재하는 데이터베이스입니다.[%s]\n", temp);
		}
		if (option == 2) {	//use
			if (useDB(temp) ==  0) {
				printf("데이터베이스 접속 성공! [%s]\n", temp);
				break;
			}
			else printf("존재하지 않는 데이터베이스입니다.\n");
		}
		if (option == 3) {	//show
			showDbs();
		}
		if (option == 4) {	//drop
			table* table = dropDB(temp);
			if(table == NULL)	printf("존재하지 않는 데이터베이스입니다.\n");
			else				dropAllTb(table);

		}
		if (option == 0) {
			printf("로그아웃합니다\n");
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



