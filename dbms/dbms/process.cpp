#include "process.h"

int loginMenu() {	// (프로그램 종료면 -1을 리턴)
	while (1) {
		printf("1. 회원가입, 2. 로그인, 3. 사용자 목록, 4. 회원탈퇴, 0. 프로그램 종료\n");
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
			//회원탈퇴
		}
		else {
			printf("[%s]\n", input);
		}
	}
	return 0;
}

int sign_in() {	// 로그인 (돌아가기면 -1을 리턴)
	char* id = NULL;
	while (1) {
		printf("\n========== 로그인 ========== (0. 돌아가기)\n");
		printf("ID : ");
		id = getString();
		if (strcmp(id, "0") == 0) return -1;
		if (isDupId(id)) {
			printf("PW : ");
			char* pw = getString();
			if (strcmp(pw, "0") == 0) return -1;
			if (checkPw(id, pw)) break;
		}
		else printf("존재하지 않는 아이디입니다\n");
	}
	printf("로그인 성공 [%s]\n", id);
	return 0;
}

void sign_up() {	// 회원가입 (돌아가기면 -1을 리턴)
	while (1) {
		printf("\n========== 회원 가입 ========== (0. 돌아가기)\n");
		printf("ID : ");
		char* id = getString();
		if (strcmp(id, "0") == 0) break;
		if (!isDupId(id)) {
			printf("PW : ");
			char* pw = getString();
			if (strcmp(pw, "0") == 0) break;
			create_user(id, pw, 1);
			printf("회원 가입 성공 [%s]\n", id);
			break;
		}
	}
}

int DBMenu() { // (로그아웃이면 -1을 리턴)
	while (1) {
		printf("\n========== 데이터베이스 쿼리를 입력하세요. ==========\n");
		char* input = getString();
		// 1.create, 2. use, 3. show, 4. drop, 0. logout
		int option = dbParsing(input);
		if (option == -1) printf("잘못된 쿼리입니다\n");
		char* temp = NULL;
		char* cmd = strtok_s(NULL, " ", &temp);
		if (option == 1) {	//create
			char* cmd = strtok_s(NULL, " ", &temp);
			if (createDB(temp)) printf("데이터베이스 생성 성공! [%s]\n", temp);
			else				printf("이미 존재하는 데이터베이스입니다.\n");
		}
		if (option == 2) {	//use
			if (useDB(temp)) {
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
	return 0;
}

void dropAllTb(table* tb) {
	table* nextTb = tb->link;
	while (tb != NULL) {
		dropTB(tb);
		tb = nextTb;
	}
}



