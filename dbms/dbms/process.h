#ifndef _PROCESS_
#define _PROCESS_
#include "common.h"
#include "structs.h"
#include "user.h"
#include "database.h"
#include "table.h"
#include "parsing.h"


int loginMenu();
int sign_in();
void sign_up();
int DBMenu();
void dropAllTb(table* table);
#endif