
#include "common.h"
#include "parsing.h"

int dbParsing(char* query) {
	int result = -1;
	if (query[strlen(query) - 1] != ';') return -1;
	else {
		query[strlen(query) - 1] = '\0';
		char* temp = NULL;
		char* cmd = strtok_s(NULL, " ", &temp);
		if (_strcmpi(cmd, "create") == 0) {	// create database testdb1;
			cmd = strtok_s(NULL, " ", &temp);
			if (_strcmpi(cmd, "database") == 0) {
				cmd = strtok_s(NULL, " ", &temp);
				if (cmd != NULL && temp == NULL) result = 1;
			}
		}
		if (_strcmpi(cmd, "use") == 0) {		// use testdb1;
			cmd = strtok_s(NULL, " ", &temp);
			if (cmd != NULL && temp == NULL) result = 2;
		}
		if (_strcmpi(cmd, "show") == 0) {	// show databases;
			cmd = strtok_s(NULL, " ", &temp);
			if (_strcmpi(cmd, "databases") == 0 && temp == NULL) result = 3;
			
		}
		if (_strcmpi(cmd, "drop") == 0) {
			cmd = strtok_s(NULL, " ", &temp);
			if (cmd != NULL && temp == NULL) result = 4;
		}
		if (_strcmpi(cmd, "logout") == 0) {
			if (temp == NULL) {
				result = 0;
			}
			else {
				printf("logout fail [%s]\n", temp);
			}
		}
	}
	return result;
}