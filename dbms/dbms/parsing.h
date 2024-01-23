#ifndef _PARSING_
#define _PARSING_ 
#include "common.h"
#include "structs.h"

enum Command {
	Error = -1,
	Create,
	Show,
	Use,
	Drop,
	Select,
	Insert,
	Delete,
	Update,
	Desc,
	Logout,
	Exit
};

int checkDbCommand(char* query);
char* createParser(char* query);
char* useParser(char* query);
char* dropParser(char* query);

int checkTbCommand(char* query);
char* tableNameParser(char* query);
column* columnInfoParser(char* query);

int getQuery(char* str);
#endif
