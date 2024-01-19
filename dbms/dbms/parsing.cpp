#include "parsing.h"


int checkCommand(char* query) {
	int result = Error;
	if (_strcmpi(query, "logout") == 0) return Logout;

	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (_strcmpi(cmd, "create") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && context[0] == '\0') {
				result = Create;
			}
		}
		else if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			printf("(Table Query) cmd:[%s], context:[%s]\n", cmd, context);

		}
	}
	else if (_strcmpi(cmd, "show") == 0) {
		cmd = strtok_s(NULL, " ", &context);	// database name
		if (_strcmpi(cmd, "databases") == 0 && context[0] == '\0') result = Show;
	}
	else if (_strcmpi(cmd, "use") == 0) {
		if (_strcmpi(strtok_s(NULL, " ", &context), "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && context[0] == '\0') result = Use;
		}
	}
	else if (_strcmpi(cmd, "drop") == 0) {
		if (_strcmpi(strtok_s(NULL, " ", &context), "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && context[0] == '\0') result = Drop;
		}	
	}
	return result;
}

char* createParser(char* query) {	// create 쿼리문을 파싱해서 db명을 리턴하는 함수
	char* context = NULL;
	char* token = strtok_s(query, " ", &context);
	token = strtok_s(NULL, " ", &context);
	if (_strcmpi(token, "database") == 0) {
		char* dbName = strtok_s(NULL, " ", &context);
		if (dbName != NULL && context[0] == '\0') {
			return dbName;
		}
	} else if (_strcmpi(token, "table") == 0) {
		char table[4] = { 0 };
		char* tbName = strtok_s(NULL, "(", &context);
		if (tbName != NULL && context != NULL) {
			table[0] = *tbName;
		}

	}
	return NULL;
}

char* useParser(char* query) {
	char* context = NULL;
	char* token = strtok_s(query, " ", &context);
	token = strtok_s(NULL, " ", &context);
	if (_strcmpi(token, "database") == 0) {
		char* dbName = strtok_s(NULL, " ", &context);
		if (dbName != NULL && context[0] == '\0') {
			//printf("(useParser)[%s] [%s] !!!\n", crud, dbName);
			return dbName;
		}
	}
	return NULL;
}

char* dropParser(char* query) {
	char* context = NULL;
	char* token = strtok_s(query, " ", &context);
	token = strtok_s(NULL, " ", &context);
	if (_strcmpi(token, "database") == 0) {
		char* dbName = strtok_s(NULL, " ", &context);
		if (dbName != NULL && context[0] == '\0') {
			return dbName;
		}
	}
	else if (_strcmpi(token, "table") == 0) {
		
	}
	else {
		
	}
	return NULL;
}

char* tableNameParser(char* query) {
	char* tbName = NULL;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (_strcmpi(cmd, "create") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			if (cmd != NULL && context != NULL) tbName = cmd;
		}

	} else if (_strcmpi(cmd, "insert") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			if (cmd != NULL && context != NULL) tbName = cmd;
		}
	}
	else if (_strcmpi(cmd, "select") == 0) {
		cmd = strtok_s(NULL, " ", &context);  // [*] [from tb1]
		cmd = strtok_s(NULL, " ", &context);  // [from] [tb1]
		if (_strcmpi(cmd, "from") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			if (cmd != NULL && context == NULL) tbName = context;
		}
	}
	else if (_strcmpi(cmd, "delete") == 0) {

	}
	else if (_strcmpi(cmd, "drop") == 0) {

	}
	return tbName;
}

char* tableInfoParser(char* query) {


	return NULL;
}



char* getQuery() {
	char* query = getString();
	if (query[strlen(query) - 1] != ';') return NULL;
	else query[strlen(query) - 1] = '\0';
	return query;
}