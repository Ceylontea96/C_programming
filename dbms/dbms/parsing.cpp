#include "parsing.h"



int dbParsing(char* query) {
	int result = -1;
	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (cmd == NULL) return result;

	else if (_strcmpi(cmd, "create") == 0) {	// create database testdb1;
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);
			//if (context[0] == '\0') printf("test");
			if (cmd != NULL && context[0] == '\0') result = 1;
		}
	}
	else if (_strcmpi(cmd, "use") == 0) {		// use testdb1;
		cmd = strtok_s(NULL, " ", &context);
		if (cmd != NULL && context[0] == '\0') result = 2;
	}
	else if (_strcmpi(cmd, "show") == 0) {	// show databases;
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "databases") == 0 && context[0] == '\0') result = 3;

	}
	else if (_strcmpi(cmd, "drop") == 0) {	// drop database testdb1;
		cmd = strtok_s(NULL, " ", &context);
		if (_strcmpi(cmd, "database") == 0) {
			cmd = strtok_s(NULL, " ", &context);
			if (cmd != NULL && context[0] == '\0') result = 4;
		}
	}
	else if (_strcmpi(cmd, "logout") == 0) {
		if (context[0] == '\0') {
			result = 0;
		}
	}

	return result;
}

int checkCommand(char* query) {
	int result = Error;
	if (query[strlen(query) - 1] != ';') return Error;
	else query[strlen(query) - 1] = '\0';

	char* context = NULL;
	char* cmd = strtok_s(query, " ", &context);
	if (_strcmpi(cmd, "create") == 0) {
		if(_strcmpi(strtok_s(NULL, " ", &context), "database") == 0)
		cmd = strtok_s(NULL, " ", &context);	// database name
		if (cmd != NULL && context[0] == '\0') result = Create;
	}
	else if (_strcmpi(cmd, "show") == 0) {
			cmd = strtok_s(NULL, " ", &context);	// database name
		if (cmd != NULL && context[0] == '\0') result = Show;
	}
	else if (_strcmpi(cmd, "use") == 0) {
		if (_strcmpi(strtok_s(NULL, " ", &context), "database") == 0)
			cmd = strtok_s(NULL, " ", &context);	// database name
		if (cmd != NULL && context[0] == '\0') result = Use;
	}
	else if (_strcmpi(cmd, "drop") == 0) {
		if (_strcmpi(strtok_s(NULL, " ", &context), "database") == 0)
			cmd = strtok_s(NULL, " ", &context);	// database name
		if (cmd != NULL && context[0] == '\0') result = Drop;
	}
}

char* createParser(char* query) {	// create 쿼리문을 파싱해서 db명을 리턴하는 함수
	char* context = NULL;
	char* crud = strtok_s(query, " ", &context);
	crud = strtok_s(NULL, " ", &context);
	if (_strcmpi(crud, "database") == 0) {
		crud = strtok_s(NULL, " ", &context);
		if (crud != NULL && context[0] == NULL) {
			printf("crud[%s], context[%s]\n", crud, context);
			char dbName[MAX] = { 0 };
			sprintf_s(dbName, sizeof(crud)-1, "%s", crud);
			printf("crud[%s], context[%s], dbName[%s]\n", crud, context, dbName);
			return dbName;
		}
	} else if (_strcmpi(crud, "table") == 0) {

	}
	return NULL;
}

char* useParser(char* query) {
	char* context = NULL;
	char* crud = strtok_s(query, " ", &context);
	crud = strtok_s(NULL, " ", &context);
	if (_strcmpi(crud, "database") == 0) {
		crud = strtok_s(NULL, " ", &context);
		if (crud != NULL && context[0] == '\0') {
			char* dbName = { 0 };
			strncpy_s(dbName, strlen(crud), crud, strlen(crud) - 1);
			return dbName;
		}
	}
	return NULL;
}