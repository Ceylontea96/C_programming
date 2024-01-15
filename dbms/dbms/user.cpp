#include "user.h"

int loginMenu() {
	while (1) {
		printf("\nȸ������ �Ǵ� �α����� �������ּ���. (0. ���α׷� ����)\n");
		printf("- 1. ȸ������\n");
		if (userTop->link != NULL) printf("- 2. �α���\n");

		char* input = getString();
		if (strcmp(input, "0") == 0) {
			return -1;
		}
		else if (strcmp(input, "1") == 0) {	// ȸ������
			sign_up();
			continue;
		}
		else if (strcmp(input, "2") == 0) {	// �α���
			sign_in();
			return 0;
		}
		else continue;
	}
	return 1;
}

void sign_up() {	// ȸ������ �Լ�
	printf("======== ȸ������ ========\n");
	printf("���̵� �Է����ּ��� (0. ���θ޴��� ���ư���)\n");
	while (1) {
		char* id = getString();
		if (strcmp(id, "0") == 0) return;
		if (isDupId(id)) printf("�̹� �����ϴ� ���̵��Դϴ�.\n");
		else {
			printf("��й�ȣ�� �Է����ּ���.\n");
			char* pw = getString();
			if (strcmp(id, "0") == 0) return;
			create_user(id, pw, 1);
			printf("[ȸ������ ����!]\n");
			break;
		}
	}
}

void sign_in() {	// �α��� �Լ�
	printf("======== �α��� ========\n");
	printf("���̵� �Է����ּ��� (0. ���θ޴��� ���ư���)\n");
	while (1) {
		char* id = getString();
		if (strcmp(id, "0") == 0) return;
		if (!isDupId(id)) printf("�������� �ʴ� ���̵��Դϴ�.\n");
		else {
			while (1) {
				char* pw = getString();
				if (strcmp(id, "0") == 0) return;
				if (checkPw(id, pw)) {
					printf("[�α��� ����!]\n");
					return;
				}
				else printf("[��й�ȣ�� �ٸ��ϴ�]\n");
			}
		}
	}
}

bool isDupId(char* id) {	// ID �ߺ����� Ȯ�� �Լ�(�ߺ��̸� TRUE, �ߺ��� �ƴϸ� FALSE�� return)
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

bool checkPw(char* id, char* pw) {	// ��й�ȣ ���� �Լ�(�α��� ������ true, ���н� false�� return)
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
	printf("======== �α��� ========\n");
	printf("���̵� �Է����ּ��� (0. ���θ޴��� ���ư���)\n");
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
