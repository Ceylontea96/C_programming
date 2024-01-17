#include "table.h"

table* tableTop;
column* columnTop;
data* dataTop;

bool isDupTb(char* tbName) { // DB 중복 검증 함수(DB 존재 시 true, 아닐시 false를 return)
	bool result = false;
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

	if (tableTop == NULL) tableTop = newTb;
	else {
		table* tb = tableTop;
		while (tb->link != NULL) {
			tb = tb->link;
		}
		tb->link = newTb;
	}
	return 0;
}

column* dropTB(char* tbName) {
	table* preTb = tableTop;
	table* tb = tableTop;
	column* comlumn = NULL;
	while (tb != NULL) {
		if (strcmp(tbName, tb->tbname) == 0) {
			if (preTb == tableTop) {
				tableTop = tb->link;
			}
			comlumn = tb->clink;
			free(tb);
			break;
		}
		preTb = tb;
		tb = tb->link;
	}
	return comlumn;
}

column* dropTB(table* tb) {
	table* nowTb = tableTop;
	table* nextTb = tb->link;
	column* comlumn = tb->clink;
	free(nowTb);
	tableTop = nextTb;
	return comlumn;
}

int showTbs() {
	printf("========== Table 목록 ==========\n");
	int index = 1;
	if (tableTop->link != NULL) {
		table* tb = tableTop;
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