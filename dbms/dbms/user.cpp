#include "user.h"

int loginMenu() {
	while (1) {
		printf("\n회원가입 또는 로그인을 진행해주세요. (0. 프로그램 종료)\n");
		printf("- 1. 회원가입\n");
		if (userTop->link != NULL) printf("- 2. 로그인\n");

		char* input = getString();
		if (strcmp(input, "0") == 0) {
			return -1;
		}
		else if (strcmp(input, "1") == 0) {	// 회원가입
			sign_up();
			continue;
		}
		else if (strcmp(input, "2") == 0) {	// 로그인
			sign_in();
			return 0;
		}
		else continue;
	}
	return 1;
}

void sign_up() {	// 회원가입 함수
	printf("======== 회원가입 ========\n");
	printf("아이디를 입력해주세요 (0. 메인메뉴로 돌아가기)\n");
	while (1) {
		char* id = getString();
		if (strcmp(id, "0") == 0) return;
		if (isDupId(id)) printf("이미 존재하는 아이디입니다.\n");
		else {
			printf("비밀번호를 입력해주세요.\n");
			char* pw = getString();
			if (strcmp(id, "0") == 0) return;
			create_user(id, pw, 1);
			printf("[회원가입 성공!]\n");
			break;
		}
	}
}

void sign_in() {	// 로그인 함수
	printf("======== 로그인 ========\n");
	printf("아이디를 입력해주세요 (0. 메인메뉴로 돌아가기)\n");
	while (1) {
		char* id = getString();
		if (strcmp(id, "0") == 0) return;
		if (!isDupId(id)) printf("존재하지 않는 아이디입니다.\n");
		else {
			while (1) {
				char* pw = getString();
				if (strcmp(id, "0") == 0) return;
				if (checkPw(id, pw)) {
					printf("[로그인 성공!]\n");
					return;
				}
				else printf("[비밀번호가 다릅니다]\n");
			}
		}
	}
}

bool isDupId(char* id) {	// ID 중복여부 확인 함수(중복이면 TRUE, 중복이 아니면 FALSE를 return)
	bool result = true;
	if (userTop->link == NULL) result = false;
	else {
		user* user = userTop->link;
		while (user != NULL && user->username != NULL) {
			if (strcmp(id, user->username) == 0) break;
			user = user->link;
		}
		result = false;
	}
	return result;
}

bool checkPw(char* id, char* pw) {	// 비밀번호 검증 함수(로그인 성공시 true, 실패시 false를 return)
	user* user = userTop->link;
	while (user != NULL && user->username != NULL) {
		if (strcmp(id, user->username) == 0) {
			if (strcmp(pw, user->pwd) == 0) {
				login_user = user;
				return true;
			}
		}
		user = user->link;
	}
	return false;
}

void create_user(char* id, char* pw, int right) {
	user* newUser = (user*)malloc(sizeof(user));
	strcpy_s(newUser->username, MAX, id);
	strcpy_s(newUser->pwd, MAX, pw);
	if(right == 1)	strcpy_s(newUser->right, MAX, "root");
	else			strcpy_s(newUser->right, MAX, "guest");
	newUser->dlink = NULL;
	newUser->link = NULL;

	if (userTop->link == NULL) userTop->link = newUser;
	else {
		user* user = userTop->link;
		while (user->link != NULL) {
			user = user->link;
		}
		user->link = newUser;
	}
}


void delete_user() {
	/*
	printf("======== 로그인 ========\n");
	printf("아이디를 입력해주세요 (0. 메인메뉴로 돌아가기)\n");
	user* user = userTop->link;
	char* id = getString();
	if (strcmp(id, "0") == 0) {

	}
	while (user != NULL && user->username != NULL) {
		if (strcmp(id, user->username) == 0) {
			if (strcmp(pw, user->pwd) == 0) {
				login_user = user;
				return true;
			}
		}
		user = user->link;
	}
	return false;
	*/
}
