#include "table.h"

int tableMenu() { // ��ɾ� ���� 1.create, 2.show, 3.drop, 4.insert, 5.update, 6.delete, 7.select, 8.exit
	int result = -1;
	while (1) {
		printf("\n========== ������ �Է��ϼ���. ==========\n");
		char* input2 = NULL;
		char* input = getString();
		if (strcmp(input, "0") == 0) return 5; // ���ư���
		if (input[strlen(input) - 1] != ';') {
			printf("�߸��� ��ɾ��Դϴ�. �ٽ� �Է����ּ���(';' ������)\n ");
			continue;
		}
		char* tbName = { 0 };
		char* temp = strtok_s(input, " ", &input2);
		if (input2 == NULL) {
			printf("�߸��� ��ɾ��Դϴ�. �ٽ� �Է����ּ���\n");
			continue;
		}
		else input2[strlen(input2) - 1] = '\0';

		if (strcmp(temp, "create") == 0) {
			temp = strtok_s(NULL, " ", &input2);
			//create table tb1(id varchar(100), pwd varchar(100), no int(10));
			if (strcmp(temp, "table") == 0) {
				if (createTB(input2) != -1) {

				}
			}


		} else if (strcmp(temp, "show") == 0) {
			if (strcmp(temp, "tables") == 0) {
				showTbs();
				continue;
			}

		}
		else if (strcmp(temp, "drop") == 0) {
			temp = strtok_s(NULL, " ", &input2);
			if (strcmp(temp, "table") == 0) {
				tbName = input2;
				printf("test [%s]\n", tbName);
				if (!isDupTb(tbName)) {
					printf("�������� �ʴ� table�Դϴ�.\n");
					continue;
				}
				if (dropTB(tbName))	printf("table ���� ����[%s]\n", tbName);
				//else					printf("table ���� ����[%s]\n", dbName);
				continue;
			}
		}
		else if (strcmp(temp, "insert") == 0) {


		}
		else if (strcmp(temp, "update") == 0) {


		}
		else if (strcmp(temp, "delete") == 0) {


		}
		else if (strcmp(temp, "select") == 0) {


		}
		else if (strcmp(temp, "exit") == 0) {
			return -1;

		}

	}
	


	return 0;
}

bool isDupTb(char* tbName) { // DB �ߺ� ���� �Լ�(DB ���� �� true, �ƴҽ� false�� return)
	bool result = true;
	if (tableTop->link != NULL) {
		table* tb = tableTop->link;
		while (tb != NULL && tb->tbname != NULL) {
			if (strcmp(tbName, tb->tbname) == 0) {
				result = true;
				break;
			}
			tb = tb->link;
		}
	}
	return result;
}

int createTB(char* tbName) {
	table* newTb = (table*)malloc(sizeof(table));
	strcpy_s(newTb->tbname, MAX, tbName);
	newTb->clink = NULL;
	newTb->link = NULL;

	if (tableTop->link == NULL) tableTop->link = newTb;
	else {
		table* tb = tableTop->link;
		while (tb->link != NULL) {
			tb = tb->link;
		}
		tb->link = newTb;
	}
	return 0;
}

int dropTB(char* tbName) {
	table* preTb = NULL;
	table* tb = tableTop->link;
	if (tableTop->link == NULL || !isDupTb(tbName)) return -1;
	while (tb->link != NULL) {
		if (strcmp(tbName, tb->tbname) == 0) {
			if (preTb == NULL) {
				tableTop->link = tb->link;
				//using_db->tlink = tableTop->link; �ذ��ؾ���
			}
			else {
				preTb->link = tb->link;
			}
			//�÷� �������� �����ϴ� �Լ� �߰�
			//deleteColumn();
			free(tb);
			break;
		}
		preTb = tb;
		tb = tb->link;
	}
	return 0;
}

int showTbs() {
	printf("========== Table ��� ==========\n");
	int index = 1;
	if (tableTop->link == NULL) return -1;
	else {
		table* tb = tableTop->link;
		while (tb != NULL && tb->tbname != NULL) {
			printf("%d. %s\n", index, tb->tbname);
			tb = tb->link;
			++index;
		}
	}
	printf("==================================\n");
	return 0;
}

int insertTb() {


	return 0;
}

int updateTb() {
	return 0;
}

int deleteTb() {
	return 0;
}

int selectTb() {
	return 0;
}