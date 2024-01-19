#ifndef _TABLE_
#define _TABLE_ 
#include "common.h"
#include "structs.h"

extern table* tableTop;
extern column* columnTop;
extern data* dataTop;

bool isDupTb(char* tbName);
void createTable(char* tbName);
column* dropTable(char* tbName);
column* dropTable(table* tb);
void showTables();

#endif
