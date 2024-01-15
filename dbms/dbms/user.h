#pragma once
#include "common.h"

typedef struct user {
	char username[MAX];	// ���� ���̵�
	char pwd[MAX];		// ���� ��й�ȣ
	char right[MAX];	// ���� ����(root, guest)
	struct database* dlink;
	struct user* link;
} user;

typedef struct userHead {
	struct user* link;
} userHead;

extern user* login_user;
extern userHead* userTop;

int loginMenu();
void sign_up();
void sign_in();
bool isDupId(char* id);
bool checkPw(char* id, char* pw);
void create_user(char* id, char* pw, int right);
void delete_user();
