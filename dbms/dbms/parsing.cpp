#include "parsing.h"


int checkDbCommand(char* query) {
	int result = Error;
	if (_strcmpi(query, "logout") == 0) return Logout;

	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (_strcmpi(cmd, "create") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && (context == NULL || context[0] == '\0')) {
				result = Create;
			}
		}
	}
	else if (_strcmpi(cmd, "show") == 0) {	//
		cmd = strtok_s(NULL, " ", &context);	// database name
		if (_strcmpi(cmd, "databases") == 0 && (context == NULL || context[0] == '\0')) result = Show;
	}
	else if (_strcmpi(cmd, "use") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && (context == NULL || context[0] == '\0')) result = Use;
		}
	}
	else if (_strcmpi(cmd, "drop") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && (context == NULL || context[0] == '\0')) result = Drop;
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
		if (dbName != NULL && (context == NULL || context[0] == '\0')) {
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
		if (dbName != NULL && (context == NULL || context[0] == '\0')) {
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
		if (dbName != NULL && (context == NULL || context[0] == '\0' )) {
			return dbName;
		}
	}
	else if (_strcmpi(token, "table") == 0) {
		char* tbName = strtok_s(NULL, " ", &context);
		if (tbName != NULL && (context == NULL || context[0] == '\0')) {
			return tbName;
		}
	}
	return NULL;
}

int checkTbCommand(char* query) {
	int result = Error;
	if (_strcmpi(query, "logout") == 0) return Logout;

	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (_strcmpi(cmd, "create") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			return Create;
		}
	}
	else if (_strcmpi(cmd, "show") == 0) {	//
		cmd = strtok_s(NULL, " ", &context);	// database name
		if (_strcmpi(cmd, "tables") == 0 && (context == NULL || context[0] == '\0')) result = Show;
	}
	else if (_strcmpi(cmd, "use") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && (context == NULL || context[0] == '\0')) result = Use;
		}
	}
	else if (_strcmpi(cmd, "drop") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
			if (cmd != NULL && (context == NULL || context[0] == '\0')) result = Drop;
		}
	}
	else if (_strcmpi(cmd, "select") == 0) {
		cmd = strtok_s(NULL, " ", &context);	// [*][from tb1 ~]
		cmd = strtok_s(NULL, " ", &context);	// [from] [tb1~]
		if (_strcmpi(cmd, "from") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// [tb1] [~~~]
			if (cmd != NULL) result = Select;
		}
	}
	else if (_strcmpi(cmd, "insert") == 0) {
		cmd = strtok_s(NULL, " ", &context);	// [into] [tb1(~~~)]
		if (_strcmpi(cmd, "into") == 0) {
			cmd = strtok_s(NULL, "(", &context);	// [tb1] [~~~]
			if (cmd != NULL) result = Insert;
		}
	}
	else if (_strcmpi(cmd, "update") == 0) {
		cmd = strtok_s(NULL, " ", &context);	// [tb1] [set id = 'test', no = 3]
		cmd = strtok_s(NULL, " ", &context);	// [set] [id = 'test', no = 3]
		if (_strcmpi(cmd, "set") == 0 && context != NULL) result = Update;
	}
	else if (_strcmpi(cmd, "delete") == 0) {

	}
	return result;
}

char* tableNameParser(char* query) {	// 쿼리문에서 테이블명만 리턴해주는 함수
	char* tbName = NULL;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (_strcmpi(cmd, "create") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			if (cmd != NULL && context != NULL) tbName = cmd;
			else {
				printf("drop cmd[%s] context[%s]\n", cmd, context);
			}
		}

	} else if (_strcmpi(cmd, "insert") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "into") == 0) {
			cmd = strtok_s(NULL, "(", &context);
			if (cmd != NULL && context != NULL) tbName = cmd;
			else {
				printf("drop cmd[%s] context[%s]\n", cmd, context);
			}
		}
	}
	else if (_strcmpi(cmd, "select") == 0) {
		cmd = strtok_s(NULL, " ", &context);  // [*] [from tb1]
		cmd = strtok_s(NULL, " ", &context);  // [from] [tb1]
		if (_strcmpi(cmd, "from") == 0) {
			cmd = strtok_s(NULL, " ", &context);
			if (cmd != NULL && (context == NULL || context[0] == '\0')) tbName = cmd;
			else {
				printf("drop cmd[%s] context[%s]\n", cmd, context);
			}
		}
	}
	else if (_strcmpi(cmd, "delete") == 0) {
		cmd = strtok_s(NULL, " ", &context);  // [from] [tb1]
		if (_strcmpi(cmd, "from") == 0) {
			cmd = strtok_s(NULL, " ", &context);
			if (cmd != NULL) tbName = cmd;
		}
	}
	else if (_strcmpi(cmd, "drop") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "table") == 0) {
			cmd = strtok_s(NULL, " ", &context);
			if (cmd != NULL && (context == NULL || context[0] == '\0')) tbName = cmd;
			else {
				printf("drop cmd[%s] context[%s]\n", cmd, context);
			}
		}
	}
	else if (_strcmpi(cmd, "update") == 0) {
		cmd = strtok_s(NULL, " ", &context);
		if (cmd != NULL && context != NULL) tbName = cmd;
		else {
			printf("drop cmd[%s] context[%s]\n", cmd, context);
		}
	}
	return tbName;
}

column* columnInfoParser(char* query) {
	column* newColumn = (column*)malloc(sizeof(column));
	if (newColumn != NULL) {


	}

	return newColumn;
}






char* getQuery() {
	char* query = getString();
	if (query[strlen(query) - 1] != ';') return NULL;
	else query[strlen(query) - 1] = '\0';
	return query;
}