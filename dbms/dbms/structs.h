#ifndef _STRUCTS_
#define _STRUCTS_ 
#include "common.h"

typedef struct user {
	char username[MAX];	// ���� ���̵�
	char pwd[MAX];		// ���� ��й�ȣ
	char right[MAX];	// ���� ����(root, guest)
	struct database* dlink;
	struct user* link;
} user;

typedef struct database {
	char dbname[MAX];
	struct table* tlink;
	struct database* link;
} database;

typedef struct table {
	char tbname[MAX];
	struct column* clink;
	struct table* link;
} table;

typedef struct column {
	int index;
	char field[MAX];
	char type[MAX];
	int size;
	struct data* dlink;
	struct column* link;
} column;

typedef struct data {
	char data[MAX];
	int check;
	struct data* link;
} data;

#endif