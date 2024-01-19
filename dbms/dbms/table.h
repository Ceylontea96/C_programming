#ifndef _TABLE_
#define _TABLE_ 
#include "common.h"
#include "structs.h"

extern table* tableTop;
extern column* columnTop;
extern data* dataTop;

table* createTable(char* tbName);
column* dropTable(char* tbName);
column* dropTable(table* tb);
void showTables();
table* getTableByName(char* tbName);
void createColumn(char* name, char* type, int size);
void selectTable(char* tbName, char* option);

void createColumn(char* name, char* type, char* size);


#endif
