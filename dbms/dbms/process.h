#ifndef _PROCESS_
#define _PROCESS_

#include "common.h"
#include "structs.h"
#include "user.h"
#include "database.h"
#include "table.h"
#include "parsing.h"

#define EXIT -1
#define LOGIN 0
#define	DATABASE 1
#define	TABLE 2

extern int SYSTEM_STATUS;

void loginMenu();
void sign_up();
void sign_in();
void delete_account();
void DBMenu();

void createDb(char* dbName);
void useDb(char* dbName);
void dropDb(char* dbName);

void tableMenu();

void tableCreateParser(char* query);
void tableInsertParser(char* query);
int InsertParser(table* tb, char* target, char* values);
void tableSelectParser(char* query);
void tableDeleteParser(char* query);
void dropTB(char* tbName);


void dropAllDb(database* databaseHead);
void dropAllTb(table* tableHead);
void dropAllColumn(column* columnHead);
void dropAllData(data* dataHead);

#endif