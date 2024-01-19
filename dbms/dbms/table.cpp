#include "table.h"

table* tableTop = NULL;
table* using_tb = NULL;
column* columnTop = NULL;
data* dataTop = NULL;

table* createTable(char* tbName) {
	table* newTb = (table*)malloc(sizeof(table));
	if (newTb != NULL) {
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
	}
	return newTb;
}

column* dropTable(char* tbName) {
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

column* dropTable(table* tb) {
	table* nowTb = tableTop;
	table* nextTb = tb->link;
	column* comlumn = tb->clink;
	free(nowTb);
	tableTop = nextTb;
	return comlumn;
}

void showTables() {
	printf("========== Table 목록 ==========\n");
	int index = 1;
	if (tableTop == NULL) printf("[Table 목록 없음]\n");
	else {
		table* tb = tableTop;
		while (tb != NULL && tb->tbname != NULL) {
			printf("%d. %s\n", index, tb->tbname);
			tb = tb->link;
			++index;
		}
	}
	printf("==================================\n");
}

table* getTableByName(char* tbName) {
	table* target_tb = NULL;
	if (tableTop != NULL) {
		table* tb = tableTop;
		while (tb != NULL && tb->tbname != NULL) {
			if (strcmp(tbName, tb->tbname) == 0) target_tb = tb;
			tb = tb->link;
		}
	}
	return target_tb;
}

void createColumn(char* name, char* type, int size) {

}

void selectTable(char* tbName, char* option) {

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


void createColumn(char* name, char* type, char* size) {
	column* newColumn = (column*)malloc(sizeof(column));
	if (newColumn != NULL) {
		strcpy_s(newColumn->field, MAX, name);
		strcpy_s(newColumn->type, MAX, type);
		newColumn->size = atoi(size);
		newColumn->dlink = NULL;
		newColumn->link = NULL;

		if (columnTop == NULL) columnTop = newColumn;
		else {
			column* column = columnTop;
			while (column->link != NULL) {
				column = column->link;
			}
			column->link = newColumn;
		}
	}


}