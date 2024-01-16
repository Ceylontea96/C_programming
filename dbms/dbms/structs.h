#ifndef _STRUCTS_
#define _STRUCTS_ 
#include "common.h"

typedef struct user {
	char username[MAX];	// 유저 아이디
	char pwd[MAX];		// 유저 비밀번호
	char right[MAX];	// 유저 권한(root, guest)
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