#ifndef _TABLE_
#define _TABLE_ 
#include "common.h"
#include "structs.h"

extern table* tableTop;
extern column* columnTop;
extern data* dataTop;

bool isDupTb(char* tbName);
int createTB(char* tbName);
column* dropTB(char* tbName);
column* dropTB(table* tb);
int showTbs();

#endif
