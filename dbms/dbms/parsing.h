#ifndef _PARSING_
#define _PARSING_ 
#include "common.h"

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

int checkCommand(char* query);
char* createParser(char* query);
char* useParser(char* query);
char* dropParser(char* query);

char* tableNameParser(char* query);
char* tableInfoParser(char* query);

char* getQuery();
#endif
