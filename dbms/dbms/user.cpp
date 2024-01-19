#include "user.h"

user* login_user = NULL;
user* userTop = NULL;

user* getUserByName(char* userName) {
	user* target_user = NULL;
	if (userTop != NULL) {
		user* user = userTop;
		while (user != NULL && user->username != NULL) {
			if (strcmp(userName, user->username) == 0) target_user = user;
			user = user->link;
		}
	}
	return target_user;
}

bool isDupId(char* id) {	// ID 중복여부 확인 함수(중복이면 TRUE, 중복이 아니면 FALSE를 return)
	bool result = false;
	if (userTop != NULL) {
		user* user = userTop;
		while (user != NULL && user->username != NULL) {
			if (strcmp(id, user->username) == 0) return true;
			user = user->link;
		}
	}
	return result;
}

bool checkPw(char* id, char* pw) {	// 비밀번호 검증 함수(아이디, 비밀번호 일치시 true, 불일치시 false를 return)
	user* user = userTop;
	while (user != NULL && user->username != NULL) {
		if (strcmp(id, user->username) == 0) {
			if (strcmp(pw, user->pwd) == 0)
				return true;
		}
		user = user->link;
	}
	return false;
}

void create_user(char* id, char* pw, int right) {
	user* newUser = (user*)malloc(sizeof(user));
	if (newUser != NULL) {
		snprintf(newUser->username, MAX, id);
		strcpy_s(newUser->pwd, MAX, pw);
		if (right == 1)	strcpy_s(newUser->right, MAX, "root");
		else			strcpy_s(newUser->right, MAX, "guest");
		newUser->dlink = NULL;
		newUser->link = NULL;

		if (userTop == NULL) userTop = newUser;
		else {
			user* user = userTop;
			while (user->link != NULL) {
				user = user->link;
			}
			user->link = newUser;
		}
	}
}

database* delete_user(char* id) {
	user* preUser = NULL;
	user* user = userTop;
	database* db = NULL;
	while (user != NULL) {
		if (strcmp(id, user->username) == 0) {
			if (preUser == NULL) {
				userTop = user->link;
			}
			else {
				preUser->link = user->link;
			}
			db = user->dlink;
			free(user);
			break;
		}
		preUser = user;
		user = user->link;
	}
	return db;
}

void showUsers() {
	printf("\n ========== 사용자 목록 ========== \n");
	int index = 1;
	user* user = userTop;
	if (user == NULL) printf("[사용자 목록 없음]\n");
	while (user != NULL) {
		printf("%d. %s\n", index++, user->username);
		user = user->link;
	}
	printf("\n ================================= \n");
}