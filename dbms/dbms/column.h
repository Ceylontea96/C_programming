#pragma once
#include "common.h"

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

extern columnHead* columnTop;

