#include "database.h"

database* using_db = NULL;
database* dbTop = NULL;

database* getDbByName(char* dbName) {
	database* target_db = NULL;
	if (dbTop != NULL) {
		database* db = dbTop;
		while (db != NULL && db->dbname != NULL) {
			if (strcmp(dbName, db->dbname) == 0) target_db = db;
			db = db->link;
		}
	}
	return target_db;
}

bool isDupDb(char* dbName) { // DB 중복 검증 함수(DB 존재 시 true, 아닐시 false를 return)
	bool result = false;
	if (dbTop != NULL) {
		database* db = dbTop;
		while (db != NULL && db->dbname != NULL) {
			if (strcmp(dbName, db->dbname) == 0) {
				result = true;
				break;
			}
			db = db->link;
		}
	}
	return result;
}

void createDB(char* dbName) {
	database* newDB = (database*)malloc(sizeof(database));
	strcpy_s(newDB->dbname, MAX, dbName);
	newDB->tlink = NULL;
	newDB->link = NULL;
	//login_user->dlink = newDB;
	
	if (dbTop == NULL) {
		dbTop = newDB;
		//login_user->dlink = dbTop;
	}
	else {
		database* db = dbTop;
		while (db->link != NULL) {
			db = db->link;
		}
		db->link = newDB;
	}
}

table* dropDB(char* dbName) {
	database* preDb = dbTop;
	database* db = dbTop;
	table* tb = NULL;
	while (db != NULL) {
		if (strcmp(dbName, db->dbname) == 0) {
			if (preDb == dbTop) {
				dbTop = db->link;
			}
			tb = db->tlink;
			free(db);
			break;
		}
		preDb = db;
		db = db->link;
	}
	return tb;
}

table* dropDB(database* db) {
	database* nowDb = db;
	database* nextDb = db->link;
	table* tb = db->tlink;
	free(nowDb);
	dbTop = nextDb;
	return tb;
}

void useDB(char* dbName) {
	if (isDupDb(dbName)) {
		database* db = dbTop;
		while (db != NULL && db->dbname != NULL) {
			if (strcmp(dbName, db->dbname) == 0) {
				using_db = db;
				break;
			}
			db = db->link;
		}
	}
}

void showDbs() {
	printf("========== Database 목록 ==========\n");
	int index = 1;
	if (dbTop == NULL) printf("[Database 목록 없음]\n");
	else {
		database* db = dbTop;
		while (db != NULL && db->dbname != NULL) {
			printf("%d. %s\n", index, db->dbname);
			db = db->link;
			++index;
		}
	}
	printf("==================================\n");
}
