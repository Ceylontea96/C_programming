#ifndef _DATABASE_
#define _DATABASE_ 
#include "common.h"
#include "structs.h"

extern database* using_db;
extern database* dbTop;

bool isDupDb(char* dbName);
int createDB(char* dbName);
table* dropDB(char* dbName);
table* dropDB(database* db);
int useDB(char* dbName);
int showDbs();
#endif