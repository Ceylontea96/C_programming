#ifndef _DATABASE_
#define _DATABASE_ 
#include "common.h"
#include "structs.h"

extern database* using_db;
extern database* dbTop;

database* getDbByName(char* dbName);
bool isDupDb(char* dbName);
void createDB(char* dbName);
table* dropDB(char* dbName);
table* dropDB(database* db);
void useDB(char* dbName);
void showDbs();
#endif