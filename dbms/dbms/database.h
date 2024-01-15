#pragma once
//#include "table.h"
#include "common.h"

typedef struct database {
	char dbname[MAX];
	struct table* tlink;
	struct database* link;
} database;

typedef struct dbHead {
	struct database* link;
} dbHead;

extern database* using_db;
extern dbHead* dbTop;

int DbMenu();
bool isDupDb(char* dbName);
int createDB(char* dbName);
int deleteDB(char* dbName);
int useDB(char* dbName);
int showDbs();