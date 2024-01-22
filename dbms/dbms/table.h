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
void selectTable(char* tbName, char* option);

column* getColumnByName(table* tb, char* ColumnName);
column* createColumn(char* name, char* type, int size);


data* dropColumn(column* Column);

int createData(column* cl, char* value);


void dropData(data* dt);
#endif
