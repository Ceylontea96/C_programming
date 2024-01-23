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
void selectTable(char* tbName, char* targets, char* options);
int deleteTb(char* tbName, char* options);
column* getColumnByName(table* tb, char* ColumnName);
column* createColumn(char* name, char* type, int size);


data* dropColumn(table* tb, column* Column);
int createData(column* cl, char* value);


void dropData(column* cl, data* target_data);
void resetDataCheck(table* tb);
int getNumberOfDatas(column* cl);
void getTarget(char* targets, char** arr);

#endif
