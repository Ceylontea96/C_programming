#pragma once
//#include "column.h"
#include "common.h"

typedef struct table {
	char tbname[MAX];
	struct column* clink;
	struct table* link;
} table;

typedef struct tableHead {
	struct table* link;
} tableHead;

extern tableHead* tableTop;

int tableMenu();
bool isDupTb(char* tbName);
int createTB(char* tbName);
int deleteTB(char* tbName);
int showTbs();
