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
void dropAllTb(table* table);

#endif