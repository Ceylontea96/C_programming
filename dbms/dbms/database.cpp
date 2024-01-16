#include "database.h"

int DbMenu() { // DB 선택 단계에서 입력한 명령어를 검증하는 함수
	// 명령어 종류 1.create, 2.use, 3.show, 4.drop, 5.logout, 6.exit
	int result = -1;
	while (1) {
		printf("\n========== Database를 선택하세요. ==========\n");
		char* input2 = NULL;
		char* input = getString();

		if (input[strlen(input) - 1] != ';') {
			printf("잘못된 명령어입니다. 다시 입력해주세요(';' 생략됨)\n ");
			continue;
		}

		char* dbName = { 0 };
		char* temp = strtok_s(input, " ", &input2);
		if (input2 == NULL) {
			printf("잘못된 명령어입니다. 다시 입력해주세요\n");
			continue;
		}
		else input2[strlen(input2) - 1] = '\0';

		if (strcmp(temp, "create") == 0) {
			temp = strtok_s(NULL, " ", &input2);
			if (strcmp(temp, "database") == 0) {
				dbName = input2;
				if (isDupDb(dbName)) {
					printf("이미 존재하는 database입니다.\n");
					continue;
				}
				createDB(dbName);
				printf("Database 생성 성공[%s]\n", dbName);
				continue;
			}
		}
		else if (strcmp(temp, "use") == 0) {
			//temp = strtok_s(NULL, " ", &dbName);
			dbName = input2;
			if (!isDupDb(dbName)) {
				printf("존재하지 않는 database입니다.\n");
				continue;
			}
			if(useDB(dbName))	printf("Database 접속 성공[%s]\n", dbName);
			//else				printf("Database 접속 실패[%s]\n", dbName);
			
			return 0;
		}
		else if (strcmp(temp, "show") == 0) {
			if (strcmp(input2, "databases") == 0) {
				showDbs();
				continue;
			}
		}
		else if (strcmp(temp, "drop") == 0) {
			temp = strtok_s(NULL, " ", &input2);
			if (strcmp(temp, "database") == 0) {
				dbName = input2;
				printf("test [%s]\n", dbName);
				if (!isDupDb(dbName)) {
					printf("존재하지 않는 database입니다.\n");
					continue;
				}
				if (dropDB(dbName))	printf("Database 삭제 성공[%s]\n", dbName);
				//else					printf("Database 삭제 실패[%s]\n", dbName);
				continue;
			}
		}
		else if (strcmp(temp, "logout") == 0) {
			return 5;
		}
		else if (strcmp(temp, "exit") == 0) {
			return -1;
		}
		printf("잘못된 명령어입니다. 다시 입력해주세요\n");
	}
	/*
	while (temp != NULL) {
		temp = strtok_s(NULL, " ", &input2);
	}
	*/
	return 0;
}

bool isDupDb(char* dbName) { // DB 중복 검증 함수(DB 존재 시 true, 아닐시 false를 return)
	bool result = true;
	if (dbTop->link != NULL) {
		database* db = dbTop->link;
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

int createDB(char* dbName) {
	database* newDB = (database*)malloc(sizeof(database));
	strcpy_s(newDB->dbname, MAX, dbName);
	newDB->tlink = NULL;
	newDB->link = NULL;

	if (dbTop->link == NULL) dbTop->link = newDB;
	else {
		database* db = dbTop->link;
		while (db->link != NULL) {
			db = db->link;
		}
		db->link = newDB;
	}
	return 0;
}

int dropDB(char* dbName) {
		database* preDb = NULL;
		database* db = dbTop->link;
		if (dbTop->link == NULL || !isDupDb(dbName)) return -1;
		while (db->link != NULL) {
			if (strcmp(dbName, db->dbname) == 0) {
				if (preDb == NULL) {
					dbTop->link = db->link;
					//login_user->dlink = dbTop->link;
				}
				else {
					preDb->link = db->link;
				}
				//table정보까지 삭제하는 함수 추가
				//deleteTable();
				free(db);
				break;
			}
			preDb = db;
			db = db->link;
		}
		return 0;
}

int useDB(char* dbName) {
	if (dbTop->link == NULL) return -1;
	else {
		database* db = dbTop->link;
		while (db != NULL && db->dbname != NULL) {
			if (strcmp(dbName, db->dbname) == 0) {
				using_db = db;
				break;
			}
			db = db->link;
		}
	}
	return 0;
}

int showDbs() {
	printf("========== Database 목록 ==========\n");
	int index = 1;
	if (dbTop->link == NULL) return -1;
	else {
		database* db = dbTop->link;
		while (db != NULL && db->dbname != NULL) {
			printf("%d. %s\n", index, db->dbname);
			db = db->link;
			++index;
		}
	}
	printf("==================================\n");
	return 0;
}