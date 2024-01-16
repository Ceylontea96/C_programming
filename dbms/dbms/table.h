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

typedef struct column {
	int index;
	char field[MAX];
	char type[MAX];
	int size;
	struct column* nextcol;
	struct data* datalink;
} column;

typedef struct columnHead {
	struct column* link;
} columnHead;

typedef struct data {
	char data[MAX];
	int check;
	struct data* nextdata;
} data;

extern tableHead* tableTop;
extern columnHead* columnTop;

int tableMenu();
bool isDupTb(char* tbName);
int createTB(char* tbName);
int dropTB(char* tbName);
int showTbs();
