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
	Desc,
	Exit
};


int dbParsing(char* query);

int checkCommand(char* query);
char* createParser(char* query);
char* useParser(char* query);

#endif
