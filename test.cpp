#pragma warning(disable :4996)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define MAXLINE 1000
#define LINE 100
#define strcasecmp _stricmp
#define _CRT_SECURE_NO_WARNINGS
#define home_root "C:\\Users\\jinho\\Documents\\neo\\DBMS\\root\\"
#define office_root "C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\"

typedef struct user {
	char username[LINE];
	char pwd[LINE];
	char root[LINE];
	struct database* dlink;
	struct user* link;
} user;
user* login_user;

typedef struct uhead {
	struct user* link;
} uhead;
uhead* utop;

typedef struct database {
	char dbname[LINE];
	struct table* tlink;
	struct database* link;
} database;
database* using_db;

typedef struct dhead {
	struct database* link;
} dhead;
dhead* dtop;

typedef struct table {
	char tbname[LINE];
	struct column* clink;
	struct table* link;
} table;

typedef struct thead {
	struct table* link;
} thead;
thead* ttop;

typedef struct column {
	int index;
	char field[LINE];
	char type[LINE];
	int size;
	struct column* nextcol;
	struct data* datalink;
} column;

typedef struct chead {
	struct column* link;
} chead;
chead* ctop;

typedef struct data {
	char data[LINE];
	int check;
	struct data* nextdata;
} data;

bool getline(char[], int);
bool check_id(char*);
bool check_pwd(char*, char*);
int init_stack();
int sign_up();
int sign_in();
void pushUser(char[], char[], char[]);
bool pushDB(char[]);
bool dropDB(char[]);
bool pushTB(char[]);
bool is_table_dup(char[]);
bool dropTB(database*, char[], bool);
void delete_table_all(char[], char[]);
bool makeCL(char[], char[], char[], int);
bool insertCL(char[], char[], char[], bool);
bool insert_check(char[], char[]);
bool dropCL(char[], char[], bool);
bool updateCL(char[], char[], char[]);
void print_stack(int);
void select(char[], char[], char[]);
void select_table(char[]);
bool change_db(char[]);
void check_init(char[]);
char* substring(char*, const char*, char**);
char* substr(int, int, char*, char*);
void push_user(char[], char[], char[]);
void push_db(char[]);
void push_tb(char[]);
void make_cl(char[], char[], char[], int);
void push_cl(char[], char[], char[]);
void drop_cl(char[], char[], char[], data*, bool);
void reg_users();
void reg_db();
void reg_table();
void reg_field(char[]);
void reg_cl(char[], char[]);

FILE* userdb;
FILE* DB_ptr;
FILE* TB_ptr;
FILE* field_ptr;
FILE* column_ptr;


//메인 실행 함수
main()
{
	char databases[LINE] = { 0 };
	char input[MAXLINE] = { 0 };
	int system_flag = 1;
	int u_reg = 0, d_reg = 0;

	printf("DBMS system on\n");
	init_stack();
	int error = 0;
	error = mkdir("C:\\Users\\jinho\\Documents\\neo\\DBMS\\root", 0776);
	if (error != -1 || (error == -1 && errno == EEXIST)) {
		printf("root 폴더 생성 성공!\n");
	}
	else
	{
		printf("root 폴더 생성 실패!\n");
		return -1;
	}
	reg_users();
	while (system_flag)
	{
		printf("\n=====Please sign-up or sign-in first=====\n");
		printf("- 1. 회원가입\n");
		if (utop->link != NULL) {
			printf("- 2. 로그인\n");
		}
		printf("================menu end================\n\n");


		getline(input, MAXLINE);
		//회원가입
		if (strcmp(input, "1") == 0) {

			sign_up();
		}
		//로그인
		else if (strcmp(input, "2") == 0)
		{
			if (sign_in())
			{
				if (u_reg == 0) {
					reg_db();
					u_reg++;
				}

				while (system_flag)
				{
					char* cmd = { 0 };
					char* temp = { 0 };
					char temp1[MAXLINE] = { 0 };
					char dbname[LINE] = { 0 };

					getline(input, MAXLINE);
					if (input[strlen(input) - 1] == ';') {
						input[strlen(input) - 1] = '\0';
						strcpy_s(temp1, MAXLINE, input);
						int ccmd = check_command(temp1);		// 명령어 종류 1.create, 2.use, 3.show, 4.drop, 5.logout, 6.exit
						//create 명령어 사용시
						if (ccmd == 1) {
							cmd = strtok_s(input, " ", &temp);
							cmd = strtok_s(NULL, " ", &temp);
							if (cmd != NULL && strcasecmp(cmd, "database") == 0) {
								cmd = strtok_s(NULL, " ", &temp);
								if (cmd != NULL) {
									if (strtok_s(NULL, " ", &temp) == NULL) {
										strcpy_s(dbname, LINE, cmd);
										if (pushDB(dbname)) {
											push_db(dbname);
											printf("Query Success!\n");
										}
									}
									else
										printf("wrong command\n");
								}
								else
									printf("wrong command\n");
							}
							else {
								printf("wrong command\n");
							}

						}
						//use 명령어 사용시
						else if (ccmd == 2) {
							cmd = strtok_s(input, " ", &temp);
							cmd = strtok_s(NULL, " ", &temp);
							if (cmd != NULL && strtok_s(NULL, " ", &temp) == NULL) {
								if (change_db(cmd)) {
									// DB전환 성공
									printf("Query Success!\n");
									printf("db changed to %s\n", cmd);
									if (d_reg == 0) {
										reg_table();
										d_reg++;
									}

									while (system_flag)
									{
										getline(input, MAXLINE);
										if (input[strlen(input) - 1] == ';') {
											input[strlen(input) - 1] = '\0';
											strcpy_s(temp1, MAXLINE, input);
											char tbname[LINE];
											ccmd = check_command2(temp1);		// 명령어 종류 1.create, 2.show, 3.drop, 4.insert, 5.update, 6.delete, 7.select, 8.exit

											//create table 명령어
											if (ccmd == 1)
											{
												bool end = false;
												bool querysuccess = false;
												char field[LINE];
												char type[LINE];
												int size;
												cmd = strtok_s(input, " ", &temp);
												cmd = strtok_s(NULL, " ", &temp);
												if (cmd != NULL && strcasecmp(cmd, "table") == 0) {	//create table
													cmd = strtok_s(NULL, "(", &temp);
													if (cmd != NULL && strchr(cmd, ' ') == NULL) {
														strcpy_s(tbname, LINE, cmd); //create table tbname(
														if (!is_table_dup(tbname)) {
															//여기까지 table명과 ( 열림
															if (pushTB(tbname))
																push_tb(tbname);
															while (!end) {
																cmd = strtok_s(NULL, " ", &temp);//create table tbname( x
																if (cmd != NULL || strcmp(cmd, ")") == 0) {
																	strcpy_s(field, LINE, cmd);

																	cmd = strtok_s(NULL, "(", &temp);
																	if (cmd != NULL && strchr(cmd, ' ') == NULL) {
																		strcpy_s(type, LINE, cmd);

																		cmd = strtok_s(NULL, ")", &temp);
																		if (cmd != NULL && strchr(cmd, ' ') == NULL) {
																			size = atoi(cmd);
																			if (size == 0) {
																				printf("query failed!(size 0)\n");
																				break;
																			}
																			if (makeCL(tbname, field, type, size))
																				make_cl(tbname, field, type, size);
																			cmd = strtok_s(NULL, " ", &temp);
																			if (cmd == NULL) {
																				printf("query failed!(wrong end)\n");
																				break;
																			}
																			else if (strcmp(cmd, ")") == 0) {
																				if (strtok_s(NULL, " ", &temp) != NULL) {
																					printf("query failed!(there is another query at end of query)\n");
																					break;
																				}
																				end = true;
																				querysuccess = true;
																				printf("query success!(create table)\n");
																				break;
																				//쿼리 종료 확인
																			}
																			else if (strcmp(cmd, ",") == 0)
																				continue;
																		}
																		else {
																			printf("query failed!(there is no size or size has space)\n");
																			break;
																		}
																	}
																	else {
																		printf("query failed!(there is no type or type has space)\n");
																		break;
																	}
																}
																else {
																	printf("query failed!(there is no field name)\n");
																	break;
																}
															}//while문 종료
															if (!querysuccess) {
																dropTB(NULL, tbname, false);
																printf("transaction return\n");
															}
														}
													}
													else
														printf("query failed!(there is no table name or table name has space)\n");
												}
												else {
													printf("query failed!(need 'table' after 'create')\n");
												}
											}
											//show db, show tables 명령어
											else if (ccmd == 2) {
												cmd = strtok_s(input, " ", &temp);
												cmd = strtok_s(NULL, " ", &temp);
												if (cmd != NULL && strcasecmp(cmd, "db") == 0 && strtok_s(NULL, " ", &temp) == NULL) {
													printf("using db : %s\n", using_db->dbname);
												}
												else if (cmd != NULL && strcasecmp(cmd, "tables") == 0 && strtok_s(NULL, " ", &temp) == NULL) {
													print_stack(2);
												}
												else
													printf("wrong command\n");
											}
											//drop table 명령어
											else if (ccmd == 3)
											{
												cmd = strtok_s(input, " ", &temp);
												cmd = strtok_s(NULL, " ", &temp);
												if (cmd != NULL && strcasecmp(cmd, "table") == 0) {
													cmd = strtok_s(NULL, " ", &temp);
													if (cmd != NULL) {
														if (strtok_s(NULL, " ", &temp) == NULL)
														{
															strcpy_s(tbname, LINE, cmd);
															if (dropTB(using_db, tbname, false))
																printf("Query Success!\n");
															else
																printf("that table does not exist\n");
														}
														else
															printf("wrong command(wrong end)\n");
													}
													else
														printf("wrong command(need 'table name')\n");
												}
												else
													printf("wrong command(need 'table')\n");
											}
											//insert 명령어
											else if (ccmd == 4)
											{
												cmd = strtok_s(input, " ", &temp);
												cmd = strtok_s(NULL, " ", &temp);
												int roopcnt = 0;
												char fields[LINE] = { 0 };
												char values[LINE] = { 0 };
												char value[LINE] = { 0 };
												char field[LINE] = { 0 };
												bool roop = true;
												if (cmd != NULL && strcasecmp(cmd, "into") == 0) {
													//cmd = strtok_s(NULL, " ", &temp);
													cmd = strtok_s(NULL, " ", &temp);
													strcpy_s(tbname, LINE, cmd);
													if (temp[0] == '(') {
														cmd = strtok_s(NULL, ")", &temp);

														strncpy_s(fields, LINE, cmd + 1, strlen(cmd));
														if (strcasecmp(strtok_s(NULL, "(", &temp), " values") == 0) {
															if (temp[0] != ')') { //값이 없는 상태로 끝나는 경우 체크

																cmd = strtok_s(NULL, ")", &temp);
																strcpy_s(values, LINE, cmd);
																if (strtok_s(NULL, " ", &temp) == NULL) {

																	if (insertCL(tbname, NULL, values, false)) {
																		printf("qeury success!\n");
																	}
																	else
																		printf("query failed!(insert error)\n");
																}
																else
																	printf("query failed!(wrong end)\n");
															}
															else
																printf("query failed!(please check values)\n");
														}
													}
													else if (strcasecmp(strtok_s(NULL, "(", &temp), "values") == 0) {
														if (temp[0] != ')') { //값이 없는 상태로 끝나는 경우 체크

															cmd = strtok_s(NULL, ")", &temp);
															strcpy_s(values, LINE, cmd);
															if (strtok_s(NULL, " ", &temp) == NULL) {

																if (insertCL(tbname, NULL, values, false))
																	printf("qeury success!\n");
																else
																	printf("query failed!(insert error)\n");
															}
															else
																printf("query failed!(wrong end)\n");
														}
														else
															printf("query failed!(please check values)\n");
													}
												}
												else
													printf("query failed!(need 'into' after 'insert')\n");
											}
											//update 명령어
											else if (ccmd == 5)
											{
												char target_column[LINE], newData[LINE], options[LINE];

												cmd = strtok_s(input, " ", &temp); //update
												cmd = strtok_s(NULL, " ", &temp);
												if (cmd != NULL) {
													strcpy_s(tbname, LINE, cmd);
													cmd = strtok_s(NULL, " ", &temp);
													if (strcasecmp(cmd, "set") == 0) {
														if (strstr(temp, "where") != NULL || strstr(temp, "WHERE") != NULL) {
															char* cmd1;
															char* temp1;
															cmd1 = substring(temp, "WHERE", &temp1);
															strcpy_s(newData, LINE, cmd1);
															strcpy_s(options, LINE, temp1);
															if (updateCL(tbname, newData, options)) {
																printf("query success!\n");
															}
														}
														else {
															if (temp != NULL) {
																strcpy_s(newData, LINE, temp);
																if (updateCL(tbname, newData, NULL)) {
																	printf("query success!\n");
																}
															}
															else
																printf("query failed!(need options after 'set')\n");
														}

													}
													else
														printf("query failed!(need 'set' after table name)\n");
												}
												else
													printf("query failed!(there is no table name)\n");
											}
											//delete 명령어
											else if (ccmd == 6)
											{
												char target_column[LINE], options[LINE];

												cmd = strtok_s(input, " ", &temp); //delete
												cmd = strtok_s(NULL, " ", &temp);
												if (cmd != NULL && strcmp(cmd, "from") == 0) {
													cmd = strtok_s(NULL, " ", &temp);
													if (cmd != NULL) {
														strcpy_s(tbname, LINE, cmd);
														cmd = strtok_s(NULL, " ", &temp);
														if (cmd == NULL) {
															dropCL(tbname, NULL, false);
														}
														else//where절이 있는 경우
														{
															if (strcasecmp(cmd, "where") == 0) {
																strcpy_s(options, LINE, temp);
																if (dropCL(tbname, options, false)) {
																	printf("query success!\n");
																}
															}
															else
																printf("query failed!(need 'where' to use options)\n");
														}
													}
													else
														printf("query failed!(there is no table name or table name has space)\n");
												}
												else
													printf("query failed!(need 'from' after 'delete')\n");
											}
											//select 명령어
											else if (ccmd == 7) {

												char target_column[LINE], options[LINE];

												cmd = strtok_s(input, " ", &temp); //select
												cmd = strtok_s(NULL, " ", &temp);
												if (cmd != NULL) {
													strcpy_s(target_column, LINE, cmd);
													cmd = strtok_s(NULL, " ", &temp);
													if (strcasecmp(cmd, "from") == 0) {
														cmd = strtok_s(NULL, " ", &temp);
														strcpy_s(tbname, LINE, cmd);
														cmd = strtok_s(NULL, " ", &temp);
														if (cmd == NULL) {
															select(tbname, target_column, NULL);
														}
														else if (strcasecmp(cmd, "where") == 0) {
															if (temp != NULL) {
																strcpy_s(options, LINE, temp);
																select(tbname, target_column, options);
															}
															else
																printf("query failed!(need options to use 'where')\n");
														}
														else
															printf("query failed!(need 'where' to use options)\n");
													}
													else
														printf("query failed!(need 'from' after target field)\n");
												}
												else
													printf("query failed!(need target field after 'select')\n");

											}
											//exit 명령어
											else if (ccmd == 8) {
												printf("go back\n");
												break;
											}
											//올바르지 않은 명령어
											if (ccmd == 0)
											{
												cmd = strtok_s(input, " ", &temp);
												printf(" '%s' is wrong command\n", cmd);
											}
										}///////
										else
											printf("query failed!(need ';' at end of query)\n");

									}//table단 while문 종료
								}
								else
									printf("that db does not exist\n");
							}
							else
								printf("wrong command(need 'db name' or wrong end)\n");
						}
						//show 명령어 사용시
						else if (ccmd == 3)
						{
							cmd = strtok_s(input, " ", &temp);
							cmd = strtok_s(NULL, " ", &temp);
							if (cmd != NULL && strcasecmp(cmd, "databases") == 0 && strtok_s(NULL, " ", &temp) == NULL) {
								printf("Query Success!\n");
								print_stack(1);
							}
							else
								printf("wrong command\n");
						}
						//drop 명령어 사용시
						else if (ccmd == 4)
						{
							cmd = strtok_s(input, " ", &temp);
							cmd = strtok_s(NULL, " ", &temp);
							if (cmd != NULL && strcasecmp(cmd, "database") == 0) {
								cmd = strtok_s(NULL, " ", &temp);
								if (cmd != NULL) {
									if (strtok_s(NULL, " ", &temp) == NULL)
									{
										strcpy_s(dbname, LINE, cmd);
										if (dropDB(dbname))
											printf("Query Success!\n");
										else
											printf("that db does not exist\n");
									}
									else
										printf("wrong command\n");
								}
								else
									printf("wrong command\n");
							}
							else
								printf("wrong command\n");
						}
						//logout 명령어 사용시
						else if (ccmd == 5)
						{
							printf("로그아웃 성공\n");
							break;
						}
						//exit 명령어 사용시
						else if (ccmd == 6)
						{
							system_flag = 0;
							break;
						}
						//올바르지 않은 명령어 사용시
						if (ccmd == 0)
						{
							cmd = strtok_s(input, " ", &temp);
							printf(" '%s' is wrong command\n", cmd);
						}
					}//if
					else
						printf("query failed!(need ';' at end of query)\n");
				}
			}

		}
		// 로그인, 회원가입 외의 다른 입력값을 받았을 때
		else
		{
			printf("wrong choise\n");
		}
	}

	printf("\nDBMS system off\n");
}




//구조체 포인터 값 초기화 함수
int init_stack() {
	utop = (uhead*)malloc(sizeof(uhead));
	utop->link = NULL;
	dtop = (dhead*)malloc(sizeof(dhead));
	dtop->link = NULL;
	ttop = (thead*)malloc(sizeof(thead));
	ttop->link = NULL;
	ctop = (column*)malloc(sizeof(column));
	ctop->link = NULL;
}

//입력값을 받는 함수
bool getline(char input[], int lim) {
	int i, c = 0;
	memset(input, 0, lim);
	printf(">");

	for (i = 0; i < lim && (c = getchar()) != EOF && c != '\n'; i++)
		input[i] = c;
	if (i >= lim - 1)
	{
		printf("Out of input range\n");
		getline(input, lim);
	}
	input[i] = '\0';
	return true;
}

//회원가입 함수
int sign_up() {
	char input[MAXLINE];
	char id[LINE] = { 0 };
	char pwd[LINE] = { 0 };
	char role[LINE] = { 0 };

	printf("\n=====회원가입=====\n");
	while (1) {
		printf("ID를 입력해주세요.\n");
		getline(input, LINE);
		strcpy_s(id, LINE, input);
		if (!check_id(id)) {
			printf("사용 가능한 ID입니다.\n");
			break;
		}
		printf("이미 존재하는 ID입니다.\n");
	}

	printf("please enter Password\n");
	getline(input, LINE);
	strcpy_s(pwd, LINE, input);

	while (1)
	{
		printf("\n권한을 설정해주세요.\n");
		printf("- 1. root\n");
		printf("- 2. guest\n\n");
		getline(input, LINE);
		if (strcmp(input, "1") == 0) {
			strcpy_s(role, LINE, "root");
			break;
		}
		else if (strcmp(input, "2") == 0) {
			strcpy_s(role, LINE, "guest");
			break;
		}
		else {
			printf("1 또는 2를 입력하여 권한을 설정해주세요.\n");
		}
	}

	pushUser(id, pwd, role);
	push_user(id, pwd, role);


	printf("\n=====회원가입 완료=====\n");

	return 1;

}

/* 파일 입출력으로 회원 정보 저장 */
void push_user(char id[], char pwd[], char role[]) {

	char path[MAXLINE];
	strcpy_s(path, MAXLINE, home_root);
	strcat(path, id);
	int error = 0;
	error = mkdir(path, 0776);
	if (error == -1 && errno != EEXIST) {
		printf("user 폴더 생성 실패!\n");
		return -1;
	}

	//없을 시 최초 생성
	strcpy_s(path, MAXLINE, home_root);
	char text[MAXLINE] = "users.txt";
	strcat(path, text);
	userdb = fopen(path, "a+");
	fprintf(userdb, "%s %s %s\n", id, pwd, role);
	fclose(userdb);

	return NULL;
}


/* 파일 입출력으로 유저 구조체 복구 */
void reg_users() {
	char* id;
	char* pwd;
	char* role;
	char path[MAXLINE];
	strcpy_s(path, MAXLINE, home_root);
	char text[MAXLINE] = "users.txt";
	strcat(path, text);
	userdb = fopen(path, "r");
	if (userdb != NULL) {
		char strTemp[MAXLINE];
		char* pStr;
		char* temp;

		while (!feof(userdb))
		{
			pStr = fgets(strTemp, sizeof(strTemp), userdb);
			if (pStr == NULL)
				break;
			id = strtok_s(pStr, " ", &temp);
			pwd = strtok_s(NULL, " ", &temp);
			role = strtok_s(NULL, "\n", &temp);
			pushUser(id, pwd, role);
		}
		fclose(userdb);
	}
}

//아이디 검증
bool check_id(char* id) {
	bool dup = false;
	char* o_id;
	char path[MAXLINE];
	strcpy_s(path, MAXLINE, home_root);
	char text[MAXLINE] = "users.txt";
	strcat(path, text);
	userdb = fopen(path, "r");
	if (userdb != NULL) {
		char strTemp[255];
		char* pStr;
		char* temp;

		while (!feof(userdb))
		{
			pStr = fgets(strTemp, sizeof(strTemp), userdb);
			o_id = strtok_s(pStr, " ", &temp);
			if (strcmp(o_id, id) == 0) {
				dup = true;
				break;
			}
		}
		fclose(userdb);
	}
	else
		dup = false;
	return dup;

}

//비밀번호 및 로그인 검증
bool check_pwd(char* pwd, char* id) {
	bool check = false;
	char* o_id;
	char* o_pwd;
	char* o_role;
	char path[MAXLINE];
	strcpy_s(path, MAXLINE, home_root);
	char text[MAXLINE] = "users.txt";
	strcat(path, text);
	userdb = fopen(path, "r");
	if (userdb != NULL) {
		char strTemp[255];
		char* pStr;
		char* temp;

		while (!feof(userdb))
		{
			pStr = fgets(strTemp, sizeof(strTemp), userdb);
			o_id = strtok_s(pStr, " ", &temp);
			if (strcmp(o_id, id) == 0) {

				o_pwd = strtok_s(NULL, " ", &temp);
				if (strcmp(o_pwd, pwd) == 0) {
					o_role = strtok_s(NULL, " ", &temp);
					check = true;
					break;
				}
			}

		}
		fclose(userdb);
	}
	else
		check = false;
	/* 파일 입출력 종료*/

	for (user* cur = utop->link; cur != NULL; cur = cur->link) {
		if (strcmp(cur->username, id) == 0) {
			if (strcmp(pwd, (cur->pwd)) == 0) {
				login_user = cur;

				check = true;
				break;
			}
		}
	}
	return check;
}

//로그인
int sign_in() {
	char id[MAXLINE];
	char pwd[MAXLINE];

	printf("\n=====로그인=====\n");
	printf("ID를 입력해주세요.\n");
	getline(id, LINE);
	if (check_id(id)) {
		printf("비밀번호를 입력해주세요.\n");
		getline(pwd, LINE);
		if (check_pwd(pwd, id)) {
			printf("로그인 성공\n");
			if (login_user->dlink != NULL)
				dtop->link = login_user->dlink;
			else
				dtop->link = NULL;
			return 1;
		}
		else {
			printf("잘못된 비밀번호입니다\n");
			return 0;
		}
	}
	else {
		printf("존재하지 않는 ID입니다.\n");
		return 0;
	}

}

//User 생성
void pushUser(char id[], char pwd[], char root[]) {
	if (utop->link == NULL) {
		user* newUser = (user*)malloc(sizeof(user));
		strcpy_s(newUser->username, LINE, id);
		strcpy_s(newUser->pwd, LINE, pwd);
		strcpy_s(newUser->root, LINE, root);
		newUser->dlink = NULL;
		newUser->link = NULL;

		utop->link = newUser;
	}
	else
	{
		user* cur = utop->link;
		while (cur->link != NULL)
		{
			cur = cur->link;
		}
		user* newUser = (user*)malloc(sizeof(user));

		strcpy_s(newUser->username, LINE, id);
		strcpy_s(newUser->pwd, LINE, pwd);
		strcpy_s(newUser->root, LINE, root);
		newUser->dlink = NULL;
		newUser->link = NULL;

		cur->link = newUser;
	}
}

//DB 메모리복구
void reg_db() {
	char* dbname;
	char id[LINE];
	char db_txt_path[MAXLINE] = home_root;
	char distin[LINE] = "\\";
	char text[LINE] = "\\databases.txt";
	strcpy_s(id, LINE, login_user->username);
	strcat(db_txt_path, id);
	strcat(db_txt_path, text);
	DB_ptr = fopen(db_txt_path, "r");
	if (DB_ptr != NULL) {
		char strTemp[255];
		char* pStr;
		char* temp;

		while (!feof(DB_ptr))
		{
			pStr = fgets(strTemp, sizeof(strTemp), DB_ptr);
			if (pStr == NULL)
				break;
			dbname = strtok_s(pStr, "\n", &temp);
			pushDB(dbname);
		}
		fclose(DB_ptr);
	}
}

// Database 생성
bool pushDB(char dbname[]) {

	if (dtop->link == NULL) {
		database* newDB = (database*)malloc(sizeof(database));
		strcpy_s(newDB->dbname, LINE, dbname);
		newDB->tlink = NULL;
		newDB->link = NULL;

		dtop->link = newDB;
		login_user->dlink = newDB;
	}
	else
	{
		//db 중복 확인
		for (database* dseek = dtop->link; dseek != NULL; dseek = dseek->link) {
			if (strcmp(dseek->dbname, dbname) == 0) {

				printf("[%s] is already exist!\n", dbname);
				return false;
			}
		}

		database* cur = dtop->link;
		while (cur->link != NULL)
		{
			cur = cur->link;
		}

		database* newDB = (database*)malloc(sizeof(database));
		strcpy_s(newDB->dbname, LINE, dbname);
		newDB->tlink = NULL;
		newDB->link = NULL;

		cur->link = newDB;
	}
	return true;
}

void push_db(char dbname[]) {

	char folder_path[MAXLINE] = home_root;
	char db_txt_path[MAXLINE];
	char id[LINE];
	char distin[LINE] = "\\";
	char text[LINE] = "\\databases.txt";
	strcpy_s(id, LINE, login_user->username);
	strcat(folder_path, id);
	strcpy_s(db_txt_path, MAXLINE, folder_path);
	strcat(db_txt_path, text);


	strcat(folder_path, distin);
	strcat(folder_path, dbname);
	int error = 0;
	error = mkdir(folder_path, 0776);
	if (error == -1 && errno != EEXIST) {
		printf("database 폴더 생성 실패!\n");
		return -1;
	}
	DB_ptr = fopen(db_txt_path, "a+");
	fprintf(DB_ptr, "%s\n", dbname);
	fclose(DB_ptr);

	return NULL;

}

// Datase 삭제
bool dropDB(char dbname[]) {

	bool r = false;
	if (dtop->link == NULL)
		r = false;
	else {
		database* temp = NULL;
		for (database* dseek = dtop->link; dseek != NULL; dseek = dseek->link) {
			if (strcmp(dseek->dbname, dbname) == 0) {
				dropTB(dseek, NULL, true);
				if (temp == NULL) {
					dtop->link = dseek->link;
					login_user->dlink = dtop->link;
				}
				else
					temp->link = dseek->link;
				//table정보까지 삭제하는 함수 추가


				dseek->tlink;
				free(dseek);
				r = true;
				break;
			}
			temp = dseek;
		}

	}

	/* 파일 입출력 시작*/
	char db_txt_path[MAXLINE], dbs_txt_temp[MAXLINE];//path는 새로운 txt, temp는 이전 txt
	char path[MAXLINE] = home_root;
	char text[LINE] = "\\databases1.txt";
	char text1[LINE] = "\\databases.txt";
	char id[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcat(path, id);
	strcpy_s(db_txt_path, MAXLINE, path);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id
	strcpy_s(dbs_txt_temp, MAXLINE, db_txt_path);	//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id
	char distin[LINE] = "\\";
	strcat(path, distin);
	strcat(path, dbname);

	strcat(db_txt_path, text); //C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\username\\databases1.txt
	strcat(dbs_txt_temp, text1); //C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\username\\databases.txt
	DB_ptr = fopen(db_txt_path, "a+");

	for (database* dseek = dtop->link; dseek != NULL; dseek = dseek->link) {
		fprintf(DB_ptr, "%s\n", dseek->dbname);
	}
	fclose(DB_ptr);
	remove(dbs_txt_temp);
	rename(db_txt_path, dbs_txt_temp);
	rmdir(path);
	/* 파일 입출력 종료 */

	return r;

}

bool is_table_dup(char tbname[]) {
	for (table* tseek = ttop->link; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {

			printf("[%s] is already exist!\n", tbname);
			return true;
		}
	}
	return false;
}

void reg_table() {
	char* tbname;
	char id[LINE], dbname[LINE];
	char tb_txt_path[MAXLINE] = home_root;
	char distin[LINE] = "\\";
	char text[LINE] = "\\tables.txt";
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(dbname, LINE, using_db->dbname);
	strcat(tb_txt_path, id);
	strcat(tb_txt_path, distin);
	strcat(tb_txt_path, dbname);
	strcat(tb_txt_path, text);
	TB_ptr = fopen(tb_txt_path, "r");
	if (TB_ptr != NULL) {
		char strTemp[255];
		char* pStr;
		char* temp;

		while (!feof(TB_ptr))
		{
			pStr = fgets(strTemp, sizeof(strTemp), TB_ptr);
			if (pStr == NULL)
				break;
			tbname = strtok_s(pStr, "\n", &temp);
			pushTB(tbname);
			reg_field(tbname);
		}
		fclose(TB_ptr);
	}
}

// Table 생성
bool pushTB(char tbname[]) {
	if (ttop->link == NULL) {
		table* newTB = (table*)malloc(sizeof(table));
		strcpy_s(newTB->tbname, LINE, tbname);
		newTB->clink = NULL;
		newTB->link = NULL;

		ttop->link = newTB;
		using_db->tlink = newTB;
	}
	else
	{
		table* cur = ttop->link;
		while (cur->link != NULL)
		{
			cur = cur->link;
		}

		table* newTB = (table*)malloc(sizeof(table));
		strcpy_s(newTB->tbname, LINE, tbname);
		newTB->clink = NULL;
		newTB->link = NULL;

		cur->link = newTB;
	}

	return true;
}

void push_tb(char tbname[]) {
	/* 파일 입출력 시작 */
	char tb_txt_path[MAXLINE] = home_root;
	char tb_folder_path[MAXLINE];//
	char text[LINE] = "\\tables.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(tb_txt_path, id);
	char distin[LINE] = "\\";
	strcat(tb_txt_path, distin);
	strcat(tb_txt_path, db);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db
	strcpy_s(tb_folder_path, MAXLINE, tb_txt_path);//
	strcat(tb_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tables.txt
	strcat(tb_folder_path, distin);
	strcat(tb_folder_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tb

	int error = 0;
	error = mkdir(tb_folder_path, 0776);
	if (error == -1 && errno != EEXIST) {
		printf("table 폴더 생성 실패!\n");
		return -1;
	}
	TB_ptr = fopen(tb_txt_path, "a+");
	fprintf(TB_ptr, "%s\n", tbname);
	fclose(TB_ptr);

	/* 파일 입출력 종료 */

}

//테이블 삭제(데이터베이스, 테이블명, 데이터베이스 내 테이블 전체삭제 유무)
bool dropTB(database* db, char tbname[], bool all) {
	//전체 테이블 삭제(DB 삭제시)
	char database[LINE];
	strcpy_s(database, LINE, db->dbname);
	bool r = false;
	if (all) {

		table* cur = db->tlink;
		table* next;
		while (cur != NULL)
		{
			next = cur->link;
			//컬럼지우는 내용 추가
			dropCL(cur->tbname, NULL, true);
			delete_table_all(database, cur->tbname);
			free(cur);
			cur = next;
			using_db->tlink = cur;
		}
		db->tlink = NULL;
		r = true;
		//return true;
	}
	//특정 테이블 삭제
	else {

		if (ttop->link == NULL)
			return false;
		else {
			table* temp = NULL;
			for (table* tseek = ttop->link; tseek != NULL; tseek = tseek->link) {
				if (strcmp(tseek->tbname, tbname) == 0) {
					dropCL(tbname, NULL, true);
					if (temp == NULL) {
						ttop->link = tseek->link;
						using_db->tlink = ttop->link;
					}
					else
						temp->link = tseek->link;


					free(tseek);
					r = true;
					break;
				}
				temp = tseek;
			}

		}
	}

	if (!all) {
		/* 파일 입출력 시작 */
		char tb_txt_path[MAXLINE], tbs_txt_temp[MAXLINE];//path는 새로운 txt, temp는 이전 txt
		char tb_folder_path[MAXLINE] = home_root;
		char text[LINE] = "\\tables1.txt";
		char text1[LINE] = "\\tables.txt";
		char id[LINE], dbname[LINE];
		strcpy_s(id, LINE, login_user->username);
		strcpy_s(dbname, LINE, database);
		strcat(tb_folder_path, id);
		char distin[LINE] = "\\";
		strcat(tb_folder_path, distin);
		strcat(tb_folder_path, dbname);
		strcat(tb_folder_path, distin);
		strcpy_s(tb_txt_path, MAXLINE, tb_folder_path);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\dbname\\
		
		strcpy_s(tbs_txt_temp, MAXLINE, tb_txt_path);	//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\dbname\\
		
		strcat(tb_folder_path, tbname);					//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\dbname\\tbname

		strcat(tb_txt_path, text); //C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\username\\dbname\\tables1.txt
		strcat(tbs_txt_temp, text1); //C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\username\\dbname\\tables.txt
		TB_ptr = fopen(tb_txt_path, "a+");

		for (table* tseek = ttop->link; tseek != NULL; tseek = tseek->link) {
			fprintf(TB_ptr, "%s\n", tseek->tbname);
		}
		fclose(TB_ptr);
		remove(tbs_txt_temp);
		rename(tb_txt_path, tbs_txt_temp);
		rmdir(tb_folder_path);
		/* 파일 입출력 종료 */
	}
	else {

	}
	return r;
}
void delete_table_all(char database[], char tbname[]) {

	char tb_folder_path[MAXLINE] = home_root;
	char tb_folder_temp[MAXLINE], tb_txt_path[MAXLINE];
	char id[LINE], dbname[LINE];
	char text[LINE] = "tables.txt";
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(dbname, LINE, database);
	strcat(tb_folder_path, id);
	char distin[LINE] = "\\";
	strcat(tb_folder_path, distin);
	strcat(tb_folder_path, dbname);
	strcat(tb_folder_path, distin);


	//strcpy_s(tb_folder_temp, MAXLINE, tb_folder_path);
	strcat(tb_folder_temp, distin);
	strcpy_s(tb_txt_path, MAXLINE, tb_folder_path);
	strcat(tb_folder_temp, tbname);
	rmdir(tb_folder_path);
	strcat(tb_txt_path, text);

	remove(tb_txt_path);




}

/* 필드 삭제는 어차피 테이블 전체 삭제시에만 작동하므로 테이블 폴더 삭제시 같이 삭제됨 */
void drop_field(char tbname[], char field[], table* tcur) {
	char field_folder_path[MAXLINE] = home_root;
	char field_txt_path[MAXLINE], field_txt_temp[MAXLINE];
	char text[LINE] = "\\fields.txt";
	char text1[LINE] = "\\fields.txt1";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(field_folder_path, id);
	char distin[LINE] = "\\";
	strcat(field_folder_path, distin);
	strcat(field_folder_path, db);
	strcat(field_folder_path, distin);
	strcat(field_folder_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(field_folder_path, distin);
	strcpy_s(field_txt_path, MAXLINE, field_folder_path);
	strcat(field_folder_path, field);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	rmdir(field_folder_path);
	strcat(field_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\fields.txt
	strcat(field_txt_temp, text1);

	field_ptr = fopen(field_txt_path, "a+");

	for (column* ccur = tcur->clink; ccur != NULL; ccur = ccur->nextcol) {
		if (strcmp(ccur->field, field) != 0) {
			fprintf(field_ptr, "%s\n", ccur->field);
		}
	}
	fclose(field_ptr);
	remove(field_txt_path);
	rename(field_txt_temp, field_txt_path);
}

/* 칼럼 복구 */
void reg_field(char tbname[]) {
	char field_txt_path[MAXLINE] = home_root;
	char text[LINE] = "\\fields.txt";
	char id[LINE], db[LINE], field1[LINE];
	char* field;
	char* type;
	int size;
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(field_txt_path, id);
	char distin[LINE] = "\\";
	strcat(field_txt_path, distin);
	strcat(field_txt_path, db);
	strcat(field_txt_path, distin);
	strcat(field_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(field_txt_path, text);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\fields.txt

	field_ptr = fopen(field_txt_path, "r");
	if (field_ptr != NULL) {
		char strTemp[255];
		char* pStr;
		char str_temp[LINE];
		char* temp;



		while (!feof(field_ptr))
		{
			pStr = fgets(strTemp, sizeof(strTemp), field_ptr);
			if (pStr == NULL)
				break;
			strcpy_s(str_temp, LINE, strTemp);
			field = strtok_s(str_temp, " ", &temp);
			type = strtok_s(NULL, " ", &temp);
			size = atoi(strtok_s(NULL, "\n", &temp));
			//size = strtok_s(NULL, "\n", &temp);
			makeCL(tbname, field, type, size);
			reg_cl(tbname, field);
		}

		fclose(field_ptr);
	}
}

//컬럼명(field) 생성(테이블명, 필드명, 타입, 크기)
bool makeCL(char tbname[], char field[], char type[], int size) {
	//type과 size 오류 체크
	if (strcasecmp(type, "varchar") != 0 && strcasecmp(type, "int") != 0 && strcasecmp(type, "double") != 0) {
		printf("type error\n");
		return false;
	}
	if (size == 0) {
		printf("size 0 error\n");
		return false;
	}


	table* targetTB;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			break;
		}
	}
	ctop->link = targetTB->clink;
	//첫 등록
	if (ctop->link == NULL) {
		column* newCL = (column*)malloc(sizeof(column));
		newCL->index = 0;
		strcpy_s(newCL->field, LINE, field);
		strcpy_s(newCL->type, LINE, type);
		newCL->size = size;
		newCL->nextcol = NULL;
		newCL->datalink = NULL;

		ctop->link = newCL;
		targetTB->clink = newCL;
	}
	else
	{


		column* cur = targetTB->clink;
		while (cur->nextcol != NULL)
		{
			if (strcmp(cur->field, field) == 0) {
				printf("query failed!('%s' field already exist!)\n", field);
				return false;
			}

			cur = cur->nextcol;
		}
		column* newCL = (column*)malloc(sizeof(column));
		newCL->index = 0;
		strcpy_s(newCL->field, LINE, field);
		strcpy_s(newCL->type, LINE, type);
		newCL->size = size;
		newCL->nextcol = NULL;
		newCL->datalink = NULL;

		cur->nextcol = newCL;
	}

	return true;
}

/* 컬럼명(필드) 폴더, 텍스트 파일 생성 */
void make_cl(char tbname[], char field[], char type[], int size) {
	/* 파일 입출력 시작 */
	char cl_txt_path[MAXLINE] = home_root;
	char cl_folder_path[MAXLINE];//
	char text[LINE] = "\\fields.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(cl_txt_path, id);
	char distin[LINE] = "\\";
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, db);
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcpy_s(cl_folder_path, MAXLINE, cl_txt_path);//
	strcat(cl_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\fields.txt
	strcat(cl_folder_path, distin);
	strcat(cl_folder_path, field);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field

	int error = 0;
	error = mkdir(cl_folder_path, 0776);
	if (error == -1 && errno != EEXIST) {
		printf("table 폴더 생성 실패!\n");
		return -1;
	}

	field_ptr = fopen(cl_txt_path, "a+");
	fprintf(field_ptr, "%s %s %d\n", field, type, size);
	fclose(field_ptr);

	/* 파일 입출력 종료 */
}

//insert query 오류 체크
bool insert_check(char tbname[], char fields[]) {
	bool find = false;
	table* targetTB;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			break;
		}
	}

	if (find) {
		column* cur = targetTB->clink;
		int i, j = 0;
		char field[LINE];
		char* cmd;
		char* temp;
		cmd = strtok_s(fields, ", ", &temp);

		while (cmd != NULL) {

			i = 0;
			while (i < j) {
				cur = cur->nextcol;
			}
			if (strcmp(cur->field, cmd) == 0) {

			}
			else {
				printf("query failed!(field name)\n");
				return false;
			}

			j++;
			cmd = strtok_s(NULL, ", ", &temp);
		}
	}
	return true;
}

// 파일 입출력을 통한 컬럼 데이터를 복구하기 위한 함수
void reg_cl(char tbname[], char field[]) {
	char cl_txt_path[MAXLINE] = home_root;
	char text[LINE] = "\\columns.txt";
	char id[LINE], db[LINE];
	char* ori_data;
	int size;
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(cl_txt_path, id);
	char distin[LINE] = "\\";
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, db);
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, field);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	strcat(cl_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns.txt

	column_ptr = fopen(cl_txt_path, "a+");
	if (column_ptr != NULL) {
		char strTemp[255];
		char* pStr;
		char* temp;

		while (!feof(column_ptr))
		{
			pStr = fgets(strTemp, sizeof(strTemp), column_ptr);
			if (pStr == NULL)
				break;
			ori_data = strtok_s(pStr, "\n", &temp);
			/* 데이터 구조체 재생성 */
			bool find = false;
			table* targetTB;
			for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
				if (strcmp(tseek->tbname, tbname) == 0) {
					targetTB = tseek;
					find = true;
					break;
				}
			}
			if (find) {
				column* ccur = targetTB->clink;

				while (ccur != NULL) {
					if (strcmp(ccur->field, field) == 0)
						break;
					ccur = ccur->nextcol;
				}

				data* dcur;
				while (ccur != NULL) {
					if (ccur->datalink == NULL) {
						data* newData = (data*)malloc(sizeof(data));

						newData->nextdata = NULL;
						if (ori_data == NULL) {
							ori_data = "NULL";
						}
						strcpy_s(newData->data, LINE, ori_data);
						newData->check = 0;

						ccur->datalink = newData;
					}
					else {
						dcur = ccur->datalink;
						while (dcur->nextdata != NULL) {
							dcur = dcur->nextdata;
						}

						data* newData = (data*)malloc(sizeof(data));
						newData->nextdata = NULL;
						if (ori_data == NULL) {
							ori_data = "NULL";
						}
						strcpy_s(newData->data, LINE, ori_data);

						newData->check = 0;

						dcur->nextdata = newData;
					}

					ccur = ccur->nextcol;
				}
			}
			/* 데이터 구조체 재생성 종료 */
		}
		fclose(column_ptr);
	}
}

//컬럼 삽입(테이블명, 필드명, 데이터)
bool insertCL(char tbname[], char fields[], char values[], bool reg) {
	bool find = false;
	table* targetTB;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			find = true;
			break;
		}
	}
	if (find) {
		column* ccur = targetTB->clink;
		data* dcur;
		char* cmd;
		char cmd1[MAXLINE];
		char* temp;
		int type = 0;
		cmd = strtok_s(values, ", ", &temp);
		while (ccur != NULL) {
			if (cmd == NULL || strchr(cmd, '\'')) {
				if (cmd[strlen(cmd) - 1] == '\'') { //varchar형
					for (int i = 0; i < strlen(cmd); i++) {
						cmd1[i] = *(cmd + i + 1);
					}
					cmd = &cmd1;
					cmd[strlen(cmd) - 1] = '\0';

					type = 1;
				}
				else  // varchar형인데 '가 한개만 있는 잘못된 입력
				{
					printf("varchar type error!\n");
					return false;
				}
			}
			else if (strchr(cmd, '\.') != NULL) { //double형
				strcpy_s(cmd1, LINE, cmd);
				for (int c = 0; c < strlen(cmd1); c++) {
					if (cmd1[c] == '\.')
						continue;
					if (!isdigit(cmd1[c])) {
						printf("input type error(int)\n");
						return false;
					}
				}
				type = 2;
			}
			else //int형
			{
				strcpy_s(cmd1, LINE, cmd);
				for (int c = 0; c < strlen(cmd1); c++) {
					if (!isdigit(cmd1[c])) {
						printf("input type error(int)\n");
						return false;
					}
				}
				type = 3;
			}

			if (ccur->datalink == NULL) {
				data* newData = (data*)malloc(sizeof(data));

				newData->nextdata = NULL;
				if (cmd == NULL) {
					cmd = "NULL";
				}
				if (strcasecmp(ccur->type, "varchar") == 0 && type == 1 && strlen(cmd) <= ccur->size) {
					strcpy_s(newData->data, LINE, cmd);
					if (!reg)
						push_cl(tbname, ccur->field, cmd);
				}
				else if (strcasecmp(ccur->type, "double") == 0 && type == 2) {
					strcpy_s(newData->data, LINE, cmd);
					if (!reg)
						push_cl(tbname, ccur->field, cmd);
				}
				else if (strcasecmp(ccur->type, "int") == 0 && type == 3) {
					strcpy_s(newData->data, LINE, cmd);
					if (!reg)
						push_cl(tbname, ccur->field, cmd);
				}
				else {
					printf("dismatched type or size error!\n");
					return false;
				}
				newData->check = 0;

				ccur->datalink = newData;
			}
			else {
				dcur = ccur->datalink;
				while (dcur->nextdata != NULL) {
					dcur = dcur->nextdata;
				}

				data* newData = (data*)malloc(sizeof(data));
				//memset(newData->data, '0', sizeof(newData->data));
				newData->nextdata = NULL;
				if (cmd == NULL) {
					cmd = "NULL";
				}
				if (strcasecmp(ccur->type, "varchar") == 0 && type == 1 && strlen(cmd) <= ccur->size) {
					strcpy_s(newData->data, LINE, cmd);
					if (!reg)
						push_cl(tbname, ccur->field, cmd);
				}
				else if (strcasecmp(ccur->type, "double") == 0 && type == 2) {
					strcpy_s(newData->data, LINE, cmd);
					if (!reg)
						push_cl(tbname, ccur->field, cmd);
				}
				else if (strcasecmp(ccur->type, "int") == 0 && type == 3) {
					strcpy_s(newData->data, LINE, cmd);
					if (!reg)
						push_cl(tbname, ccur->field, cmd);
				}
				else {
					printf("dismatched type error!\n");
					return false;
				}
				newData->check = 0;

				dcur->nextdata = newData;
			}

			ccur = ccur->nextcol;
			cmd = strtok_s(NULL, ", ", &temp);
		}

	}
	else
	{
		printf("[%s] does not exist!\n", tbname);
		return false;
	}
	return true;
}

void push_cl(char tbname[], char field[], char new_data[]) {

	char cl_txt_path[MAXLINE] = home_root;
	char text[LINE] = "\\columns.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(cl_txt_path, id);
	char distin[LINE] = "\\";
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, db);
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, field);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	strcat(cl_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns.txt

	column_ptr = fopen(cl_txt_path, "a+");
	fprintf(column_ptr, "%s\n", new_data);
	fclose(column_ptr);

}

void remove_cl(char tbname[], char field[]) {
	char cl_txt_path[MAXLINE] = home_root;
	char text[LINE] = "\\columns.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(cl_txt_path, id);
	char distin[LINE] = "\\";
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, db);
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, field);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	strcat(cl_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns.txt

	remove(cl_txt_path);
}

//필드 전체삭제용 함수
void remove_field(char tbname[], char field[]) {
	char field_folder_path[MAXLINE] = home_root;
	char filed_txt_path[MAXLINE];
	char text[LINE] = "\\fields.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(field_folder_path, id);
	char distin[LINE] = "\\";
	strcat(field_folder_path, distin);
	strcat(field_folder_path, db);
	strcat(field_folder_path, distin);
	strcat(field_folder_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcpy_s(filed_txt_path, MAXLINE, field_folder_path);
	strcat(field_folder_path, distin);
	strcat(field_folder_path, field);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	strcat(filed_txt_path, text);

	rmdir(field_folder_path);
	remove(filed_txt_path);
}

//테이블 내 컬럼 삭제(해당 테이블을 찾지 못했을 경우 false 리턴)
bool dropCL(char tbname[], char options[], bool all) {
	table* targetTB;
	bool find = false;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			find = true;
			break;
		}
	}
	bool r = false;
	if (!all && targetTB->clink->datalink == NULL) {
		column* c_temp = ctop->link;
		printf("there is no data to delete\n");
		while (c_temp != NULL) {
			remove_cl(tbname, c_temp->field);
			remove_field(tbname, c_temp->field);
			c_temp = c_temp->nextcol;
		}

		return false;
	}
	if (!all)
		check_init(tbname);
	if (find) {

		ctop->link = targetTB->clink;

		if (options == NULL) {

			column* cur;
			column* next;
			data* nextdata;

			for (cur = ctop->link; cur != NULL; cur = cur->nextcol) {
				data* datacur = cur->datalink;
				remove_cl(tbname, cur->field);
				while (datacur != NULL) {
					nextdata = datacur->nextdata;
					free(datacur);
					datacur = nextdata;
				}
				cur->datalink = NULL;
			}

			if (all) {
				cur = ctop->link;
				while (cur != NULL) {
					next = cur->nextcol;

					remove_field(tbname, cur->field);
					free(cur);
					cur = next;
				}
				targetTB->clink = NULL;
			}

			ctop->link = NULL;
			return true;
		}
		else if (options != NULL) {

			column* cseek = ctop->link;
			data* datacur = cseek->datalink;
			char* cmd;
			char* cmd1;
			char* temp;
			char* temp1;
			char temp0[LINE], target_column[LINE], mark[LINE], newValue[LINE], valueTemp[LINE];
			char field1[LINE], opt1[LINE], t_data1[LINE], oper[LINE];
			int opts = 0, i, j, k;

			strcpy_s(temp0, LINE, options);

			check_init(tbname);

			cseek = ctop->link;
			bool check = false;

			do {
				if (opts != 0) {
					strcpy_s(oper, LINE, cmd1);
				}
				if (opts == 0) {
					cmd1 = strtok_s(temp0, " ", &temp1);
				}
				else {
					cmd1 = strtok_s(NULL, " ", &temp1);
				}

				strcpy_s(field1, LINE, cmd1);
				/* 해당 컬럼이 존재하는지 확인 */
				bool target_correct = false;

				target_correct = false;
				while (cseek != NULL) {
					if (strcmp(cseek->field, field1) == 0) {
						target_correct = true;
						break;
					}

					cseek = cseek->nextcol;
				}

				if (!target_correct) {
					printf("query failed!(not correctly column)\n");
					return false;
				}
				/* 컬럼 확인 종료 */
				cmd1 = strtok_s(NULL, " ", &temp1);
				if (cmd1 == NULL || (strcmp(cmd1, "=") != 0 && strcmp(cmd1, "!=") != 0)) {
					printf("query failed(wrong option)\n");
					return false;
				}
				strcpy_s(opt1, LINE, cmd1);
				cmd1 = strtok_s(NULL, " ", &temp1);
				if (cmd1 == NULL) {
					printf("query failed(there is no t_data)\n");
					return false;
				}
				strcpy_s(t_data1, LINE, cmd1);
				opts++;
				i = 0;
				datacur = cseek->datalink;
				while (datacur != NULL) {
					cseek = ctop->link;
					while (cseek != NULL) {
						datacur = cseek->datalink;
						k = 0;
						j = 0;
						if (strcmp(field1, cseek->field) == 0) {
							//비교 데이터가 문자형일 경우 '' 체크와 비교를 위한 변경
							char t_data_temp[LINE];
							if (strcasecmp(cseek->type, "varchar") == 0) {
								if (t_data1[0] == '\'') {
									if (t_data1[strlen(t_data1) - 1] == '\'') { //varchar형
										for (int i = 0; i < strlen(t_data1); i++) {
											t_data_temp[i] = t_data1[i + 1];
										}
										strcpy_s(t_data1, LINE, t_data_temp);
										t_data1[strlen(t_data1) - 1] = '\0';
									}
									else  // varchar형인데 '가 한개만 있는 잘못된 입력
									{
										printf("varchar type error!\n");
										return false;
									}
								}
							}

							while (datacur != NULL) {
								check = false;
								//조건에 부합하는 데이터를 찾았을 경우
								if (strcmp(opt1, "=") == 0) {
									if (strcmp(datacur->data, t_data1) == 0)
										check = true;
								}
								else if (strcmp(opt1, "!=") == 0) {
									if (strcmp(datacur->data, t_data1) != 0)
										check = true;
								}
								if (check) {
									//조건을 찾았으면
									column* c_temp;
									c_temp = ctop->link;
									while (c_temp != NULL) {
										k = 0;
										data* data_temp;
										data_temp = c_temp->datalink;
										while (k < j) {
											data_temp = data_temp->nextdata;
											k++;
										}
										data_temp->check += 1;
										c_temp = c_temp->nextcol;
									}
								}
								datacur = datacur->nextdata;
								j++;
							}
						}
						if (datacur == NULL)
							break;
						cseek = cseek->nextcol;
					}
					i++;
				}
				//컬럼 check end
				cmd1 = strtok_s(NULL, " ", &temp1);
			} while (cmd1 != NULL);
			//전체 while end

			/* 삭제 부분 */
			i = 0;
			datacur = cseek->datalink;
			bool delete = false;
			while (datacur != NULL) {
				cseek = ctop->link;
				delete = false;
				while (cseek != NULL) {
					datacur = cseek->datalink;
					k = 0;
					column* cur;
					column* next;
					data* next_data;
					data* d_cur = cseek->datalink;
					data* d_pos = cseek->datalink;	//파일입출력 함수에 전달하기 위한 포인터
					char t_data[LINE];
					while (k < i) {
						d_cur = datacur;
						datacur = datacur->nextdata;
						k++;
					}
					if (datacur == NULL)
						break;
					if (opts == 1) {
						if (datacur->check == 1) {
							strcpy_s(t_data, LINE, datacur->data);
							if (i == 0) {
								cseek->datalink = datacur->nextdata;
								if (datacur->nextdata == NULL)
									drop_cl(tbname, cseek->field, t_data, d_pos, true);
								else
									drop_cl(tbname, cseek->field, t_data, d_pos, false);
							}
							else {
								d_cur->nextdata = datacur->nextdata;
								drop_cl(tbname, cseek->field, t_data, d_pos, false);
							}
							next_data = datacur->nextdata;
							free(datacur);

							datacur = next_data;
							delete = true;
						}
					}
					else {
						if (strcasecmp(oper, "and") == 0) {
							if (datacur->check == opts) {
								strcpy_s(t_data, LINE, datacur->data);
								if (i == 0) {
									cseek->datalink = datacur->nextdata;
									if (datacur->nextdata == NULL)
										drop_cl(tbname, cseek->field, t_data, d_pos, true);
									else
										drop_cl(tbname, cseek->field, t_data, d_pos, false);
								}
								else {
									d_cur->nextdata = datacur->nextdata;
									drop_cl(tbname, cseek->field, t_data, d_pos, false);
								}
								next_data = datacur->nextdata;
								free(datacur);

								datacur = next_data;
								delete = true;
							}
						}
						else if (strcasecmp(oper, "or") == 0) {
							if (datacur->check == 1) {
								strcpy_s(t_data, LINE, datacur->data);
								if (i == 0) {
									cseek->datalink = datacur->nextdata;
									if (datacur->nextdata == NULL)
										drop_cl(tbname, cseek->field, t_data, d_pos, true);
									else
										drop_cl(tbname, cseek->field, t_data, d_pos, false);
								}
								else {
									d_cur->nextdata = datacur->nextdata;
									drop_cl(tbname, cseek->field, t_data, d_pos, false);
								}
								next_data = datacur->nextdata;
								free(datacur);

								datacur = next_data;
								delete = true;
							}
						}
					}
					cseek = cseek->nextcol;
				}
				if (!delete) {
					i++;
				}
			}
		}
	}
	return true;
}



// 파일 입출력을 통한 컬럼 삭제 함수
void drop_cl(char tbname[], char field[], char t_data[], data* d_pos, bool first) {
	/* 파일 입출력 부분 */
	char cl_txt_path[MAXLINE] = home_root;
	char cl_txt_temp[MAXLINE];
	char text[LINE] = "\\columns.txt";
	char text1[LINE] = "\\columns1.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(cl_txt_path, id);
	char distin[LINE] = "\\";
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, db);
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, field);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	strcpy_s(cl_txt_temp, MAXLINE, cl_txt_path);
	strcat(cl_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns.txt
	strcat(cl_txt_temp, text1);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns1.txt

	if (first) {
		remove(cl_txt_path);
	}
	else {
		column_ptr = fopen(cl_txt_temp, "a+");

		for (data* datacur = d_pos; datacur != NULL; datacur = datacur->nextdata) {
			if (strcmp(datacur->data, t_data) != 0) {
				fprintf(column_ptr, "%s\n", datacur->data);
			}
		}
		fclose(column_ptr);
		remove(cl_txt_path);
		rename(cl_txt_temp, cl_txt_path);
	}

	/* 파일 입출력 종료 */
}

// 파일 입출력을 통한 컬럼 수정 함수
void update_cl(char tbname[], char field[], char t_data[], char new_data[], data* d_pos) {
	char cl_txt_path[MAXLINE] = home_root;
	char cl_txt_temp[MAXLINE];
	char text[LINE] = "\\columns.txt";
	char text1[LINE] = "\\columns1.txt";
	char id[LINE], db[LINE];
	strcpy_s(id, LINE, login_user->username);
	strcpy_s(db, LINE, using_db->dbname);
	strcat(cl_txt_path, id);
	char distin[LINE] = "\\";
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, db);
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, tbname);		//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname
	strcat(cl_txt_path, distin);
	strcat(cl_txt_path, field);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field
	strcpy_s(cl_txt_temp, MAXLINE, cl_txt_path);
	strcat(cl_txt_path, text);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns.txt
	strcat(cl_txt_temp, text1);			//C:\\Users\\neo\\Documents\\zeeno\\DBMS\\root\\id\\db\\tbname\\field\\columns1.txt

	column_ptr = fopen(cl_txt_temp, "a+");

	for (data* datacur = d_pos; datacur != NULL; datacur = datacur->nextdata) {
		if (strcmp(datacur->data, t_data) != 0) {
			fprintf(column_ptr, "%s\n", datacur->data);
		}
		else
			fprintf(column_ptr, "%s\n", new_data);
	}
	fclose(column_ptr);
	remove(cl_txt_path);
	rename(cl_txt_temp, cl_txt_path);
}

//컬럼 수정
bool updateCL(char tbname[], char newData[], char options[]) {
	table* targetTB;
	bool find = false;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			find = true;
		}
	}

	check_init(tbname);

	if (find) {

		ctop->link = targetTB->clink;
		column* cseek = ctop->link;
		data* datacur = cseek->datalink;
		data* d_pos = cseek->datalink;
		char* cmd;
		char* cmd1;
		char* temp;
		char* temp1;
		char temp0[LINE], target_column[LINE], mark[LINE], newValue[LINE], valueTemp[LINE];

		char type[LINE] = { 0 };
		char* type1 = NULL;
		if (options == NULL) {

			strcpy_s(temp0, LINE, newData);

			cseek = ctop->link;
			cmd = strtok_s(temp0, ",", &temp);

			while (cmd != NULL) {
				cmd1 = strtok_s(cmd, " ", &temp1);
				cseek = ctop->link;

				//SET절을 우선 분리
				if (cmd1 != NULL) {
					strcpy_s(target_column, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 != NULL && strcmp(cmd1, "=") == 0) {
						strcpy_s(mark, LINE, cmd1);
						cmd1 = strtok_s(NULL, " ", &temp1);
						if (cmd1 != NULL) {
							strcpy_s(newValue, LINE, cmd1);
							cmd1 = strtok_s(NULL, " ", &temp1);

							//newValue 데이터 타입 확인
							if (newValue[0] == '\'') {
								if (newValue[strlen(newValue) - 1] == '\'') { //varchar형
									for (int i = 0; i < strlen(newValue); i++) {
										valueTemp[i] = newValue[i + 1];
									}
									strcpy_s(newValue, LINE, valueTemp);
									newValue[strlen(newValue) - 1] = '\0';

									strcpy_s(type, LINE, "varchar");
									type1 = "varchar";
								}
								else  // varchar형인데 '가 한개만 있는 잘못된 입력
								{
									printf("varchar type error!\n");
									return false;
								}
							}
							else if (strchr(newValue, '\.') != NULL) { //double형
								type1 = "double";
							}
							else //int형
							{
								type1 = "int";
							}
						}
					}
					else
					{
						printf("query failed!(there is no mark)\n");
						return false;
					}
				}
				else
				{
					printf("query failed!(there is no target column)\n");
					return false;
				}

				while (cseek != NULL) {
					if (strcmp(cseek->field, target_column) == 0) {
						if (strcmp(cseek->type, type1) == 0) {
							datacur = cseek->datalink;
							while (datacur != NULL) {
								if (strcmp(type1, "varchar") == 0 && cseek->size <= strlen(newValue)) {
									printf("query failed!(unmatched data size!)\n");
									return false;
								}
								//수정부분
								//d_pos = datacur;
								update_cl(tbname, cseek->field, datacur->data, newValue, d_pos);
								strcpy_s(datacur->data, LINE, newValue);
								datacur = datacur->nextdata;
							}
						}
						else {
							printf("qeury failed!(update type error)\n");
							return;
						}
						break;
					}
					cseek = cseek->nextcol;
				}

				cmd = strtok_s(NULL, ",", &temp);

			}//cmd != null

		}
		else if (options != NULL) {

			int opts = 0, i, k, j;
			char field[LINE] = { 0 };
			char opt[LINE] = { 0 };
			char t_data[LINE] = { 0 };
			char oper[LINE];
			bool check;

			do {
				if (opts != 0) {
					strcpy_s(oper, LINE, cmd1);
				}
				if (opts == 0) {
					cmd1 = strtok_s(options, " ", &temp1);
				}
				else {
					cmd1 = strtok_s(NULL, " ", &temp1);
				}

				strcpy_s(field, LINE, cmd1);
				cmd1 = strtok_s(NULL, " ", &temp1);
				if (cmd1 == NULL || (strcmp(cmd1, "=") != 0 && strcmp(cmd1, "!=") != 0)) {
					printf("query failed(wrong option1)\n");
					return false;
				}
				strcpy_s(opt, LINE, cmd1);
				cmd1 = strtok_s(NULL, " ", &temp1);
				if (cmd1 == NULL) {
					printf("query failed(wrong t_data1)\n");
					return false;
				}
				strcpy_s(t_data, LINE, cmd1);
				opts++;
				i = 0;
				datacur = cseek->datalink;
				while (datacur != NULL) {
					cseek = ctop->link;
					while (cseek != NULL) {
						datacur = cseek->datalink;
						k = 0;
						j = 0;
						if (strcmp(field, cseek->field) == 0) {
							//비교 데이터가 문자형일 경우 '' 체크와 비교를 위한 변경
							char t_data_temp[LINE];
							if (strcasecmp(cseek->type, "varchar") == 0) {
								if (t_data[0] == '\'') {
									if (t_data[strlen(t_data) - 1] == '\'') { //varchar형
										for (int i = 0; i < strlen(t_data); i++) {
											t_data_temp[i] = t_data[i + 1];
										}
										strcpy_s(t_data, LINE, t_data_temp);
										t_data[strlen(t_data) - 1] = '\0';
									}
									else  // varchar형인데 '가 한개만 있는 잘못된 입력
									{
										printf("varchar type error!\n");
										return false;
									}
								}
							}

							while (datacur != NULL) {
								check = false;
								//조건에 부합하는 데이터를 찾았을 경우
								if (strcmp(opt, "=") == 0) {
									if (strcmp(datacur->data, t_data) == 0)
										check = true;
								}
								else if (strcmp(opt, "!=") == 0) {
									if (strcmp(datacur->data, t_data) != 0)
										check = true;
								}
								if (check) {
									//조건을 찾았으면
									column* c_temp;
									c_temp = ctop->link;
									while (c_temp != NULL) {
										k = 0;
										data* data_temp;
										data_temp = c_temp->datalink;
										while (k < j) {
											data_temp = data_temp->nextdata;
											k++;
										}
										data_temp->check += 1;
										c_temp = c_temp->nextcol;
									}
								}
								datacur = datacur->nextdata;
								j++;
							}
						}
						if (datacur == NULL)
							break;
						cseek = cseek->nextcol;
					}
					i++;
				}
				//컬럼 check end
				cmd1 = strtok_s(NULL, " ", &temp1);
			} while (cmd1 != NULL);
			//전체 while end

			/* 데이터 오류 체크를 위한 반복문 부분 */
			i = 0;
			cseek = ctop->link;
			datacur = cseek->datalink;
			bool update = false;
			update = false;
			char tc[LINE];
			strcpy_s(tc, LINE, newData);
			/* SET절을 우선 분리 */
			cmd1 = strtok_s(tc, " ", &temp1);
			while (cmd1 != NULL) {

				if (cmd1 != NULL) {
					strcpy_s(target_column, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 != NULL && strcmp(cmd1, "=") == 0) {
						strcpy_s(mark, LINE, cmd1);
						cmd1 = strtok_s(NULL, ",", &temp1);
						if (cmd1 != NULL) {
							strcpy_s(newValue, LINE, cmd1);
							cmd1 = strtok_s(NULL, " ", &temp1);

							//newValue 데이터 타입 확인
							if (newValue[0] == '\'') {
								if (newValue[strlen(newValue) - 1] == '\'') { //varchar형
									for (int i = 0; i < strlen(newValue); i++) {
										valueTemp[i] = newValue[i + 1];
									}
									strcpy_s(newValue, LINE, valueTemp);
									newValue[strlen(newValue) - 1] = '\0';

									strcpy_s(type, LINE, "varchar");
									type1 = "varchar";
								}
								else  // varchar형인데 '가 한개만 있는 잘못된 입력
								{
									printf("varchar type error!\n");
									return false;
								}
							}
							else if (strchr(newValue, '\.') != NULL) { //double형
								type1 = "double";
							}
							else //int형
							{
								type1 = "int";
							}
						}
					}
					else
					{
						printf("query failed!(there is no mark)\n");
						return false;
					}
				}
				else
				{
					printf("query failed!(there is no target column)\n");
					return false;
				}
				/* SET절 분리 종료 */

				cseek = ctop->link;
				datacur = cseek->datalink;

				while (cseek != NULL) {
					datacur = cseek->datalink;
					if (strcmp(cseek->field, target_column) == 0) {

						while (datacur != NULL) {

							if (opts == 1) {
								if (datacur->check == 1) {
									if (strcasecmp(cseek->type, type1) == 0) {
										if (strcmp(type1, "varchar") == 0 && cseek->size <= strlen(newValue)) {
											printf("query failed!(unmatched data size!)\n");
											return false;
										}
										update = true;
									}
									else {
										printf("query failed!(unmatched type!)\n");
										return false;
									}
								}
							}
							else {
								if (strcasecmp(oper, "and") == 0) {
									if (datacur->check == opts) {
										if (strcasecmp(cseek->type, type1) == 0) {
											if (strcmp(type1, "varchar") == 0 && cseek->size <= strlen(newValue)) {
												printf("query failed!(unmatched data size!)\n");
												return false;
											}
											update = true;
										}
										else {
											printf("query failed!(unmatched type!)\n");
											return false;
										}
									}
								}
								else if (strcasecmp(oper, "or") == 0) {
									if (datacur->check == 1) {
										if (strcasecmp(cseek->type, type1) == 0) {
											if (strcmp(type1, "varchar") == 0 && cseek->size <= strlen(newValue)) {
												printf("query failed!(unmatched data size!)\n");
												return false;
											}

											update = true;
										}
										else {
											printf("query failed!(unmatched type!)\n");
											return false;
										}
									}
								}
							}
							datacur = datacur->nextdata;
						}
					}
					cseek = cseek->nextcol;
				}
			}
			/* 데이터 오류 체크 end */

			strcpy_s(tc, LINE, newData);
			/* SET절을 우선 분리 */
			cmd1 = strtok_s(tc, " ", &temp1);
			while (cmd1 != NULL) {

				if (cmd1 != NULL) {
					strcpy_s(target_column, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 != NULL && strcmp(cmd1, "=") == 0) {
						strcpy_s(mark, LINE, cmd1);
						cmd1 = strtok_s(NULL, ",", &temp1);
						if (cmd1 != NULL) {
							strcpy_s(newValue, LINE, cmd1);
							cmd1 = strtok_s(NULL, " ", &temp1);

							//newValue 데이터 타입 확인
							if (newValue[0] == '\'') {
								if (newValue[strlen(newValue) - 1] == '\'') { //varchar형
									for (int i = 0; i < strlen(newValue); i++) {
										valueTemp[i] = newValue[i + 1];
									}
									strcpy_s(newValue, LINE, valueTemp);
									newValue[strlen(newValue) - 1] = '\0';

									strcpy_s(type, LINE, "varchar");
									type1 = "varchar";
								}
								else  // varchar형인데 '가 한개만 있는 잘못된 입력
								{
									printf("varchar type error!\n");
									return false;
								}
							}
							else if (strchr(newValue, '\.') != NULL) { //double형
								type1 = "double";
							}
							else //int형
							{
								type1 = "int";
							}
						}
					}
					else
					{
						printf("query failed!(there is no mark)\n");
						return false;
					}
				}
				else
				{
					printf("query failed!(there is no target column)\n");
					return false;
				}
				/* SET절 분리 종료 */

				cseek = ctop->link;
				datacur = cseek->datalink;

				/* 데이터 수정 */
				while (cseek != NULL) {
					datacur = cseek->datalink;
					if (strcmp(cseek->field, target_column) == 0) {

						while (datacur != NULL) {

							if (opts == 1) {
								if (datacur->check == 1) {
									if (strcasecmp(cseek->type, type1) == 0) {
										//수정부분
										//d_pos = datacur;
										update_cl(tbname, cseek->field, datacur->data, newValue, d_pos);
										strcpy_s(datacur->data, LINE, newValue);
										update = true;
									}
									else {
										printf("query failed!(unmatched type!)\n");
										return false;
									}
								}
							}
							else {
								if (strcasecmp(oper, "and") == 0) {
									if (datacur->check == opts) {
										if (strcasecmp(cseek->type, type1) == 0) {
											//수정부분
											//d_pos = datacur;
											update_cl(tbname, cseek->field, datacur->data, newValue, d_pos);
											strcpy_s(datacur->data, LINE, newValue);
											update = true;
										}
										else {
											printf("query failed!(unmatched type!)\n");
											return false;
										}
									}
								}
								else if (strcasecmp(oper, "or") == 0) {
									if (datacur->check == 1) {
										if (strcasecmp(cseek->type, type1) == 0) {
											//수정부분
											//d_pos = datacur;
											update_cl(tbname, cseek->field, datacur->data, newValue, d_pos);
											strcpy_s(datacur->data, LINE, newValue);
											update = true;
										}
										else {
											printf("query failed!(unmatched type!)\n");
											return false;
										}
									}
								}
							}

							datacur = datacur->nextdata;
						}
					}

					cseek = cseek->nextcol;
				}
				/* 데이터 수정 end*/
			}
			/* 데이터 수정 반복문 end*/
		}
	}
	//if (find) 종료
	return true;
}


// 내부 목록 출력
void print_stack(int t) {
	//database
	if (t == 1) {
		if (login_user->dlink == NULL) {
			printf("-no database exist!\n");
		}
		else
		{
			printf("\n======== DATABASES ========\n");
			for (database* cur = login_user->dlink; cur != NULL; cur = cur->link)
				printf("- %s\n", cur->dbname);
			printf("===========================\n");
		}
	}
	//table
	else if (t == 2) {
		if (using_db->tlink == NULL) {
			printf("-no table exist!\n");
		}
		else
		{
			printf("\n======== TABLES ========\n");
			for (table* cur = using_db->tlink; cur != NULL; cur = cur->link)
				printf("- %s -\n", cur->tbname);
			printf("=========================\n");
		}
	}
}

//테이블 내 컬럼 전체 조회
void select_table(char tbname[]) {
	bool find = false;
	if (using_db->tlink == NULL) {
		printf("-create table first!\n");
	}
	else {
		table* targetTB;
		for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
			if (strcmp(tseek->tbname, tbname) == 0) {
				targetTB = tseek;
				find = true;
				break;
			}
		}

		if (find) {
			ctop->link = targetTB->clink;


			column* cseek = ctop->link;
			data* datacur = cseek->datalink;
			int i = 0, j = 0, k = 0;

			printf("========== [%s] ==========\n", targetTB->tbname);
			while (cseek != NULL) {
				printf("%s\t", cseek->field);
				cseek = cseek->nextcol;
			}
			printf("\n");
			cseek = ctop->link;
			//세부 컬럼 출력
			while (datacur != NULL) {
				cseek = ctop->link;
				while (cseek != NULL) {

					datacur = cseek->datalink;
					k = 0;
					while (k < i) {
						datacur = datacur->nextdata;
						k++;
					}
					if (datacur == NULL)
						break;

					printf("%s\t", datacur->data);

					cseek = cseek->nextcol;

				}
				printf("\n");
				i++;
				if (datacur->nextdata == NULL)
					break;


			}
			printf("============================\n");
		}
		else
		{
			printf("[%s] does not exist!\n", tbname);
			return;
		}

	}


}

//where 조건으로 select
void select(char tbname[], char target_column[], char options[]) {

	table* targetTB;
	bool find = false;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			find = true;
		}
	}
	if (find) {

		//option 확인
		char field1[LINE] = { 0 };
		char opt1[LINE] = { 0 };
		char t_data1[LINE] = { 0 };
		char oper[LINE];
		char* cmd1;
		char* temp1;
		int opts = 0;

		column* targetCL;
		ctop->link = targetTB->clink;

		// 전체 column 조회
		if (strcmp(target_column, "*") == 0) {

			if (options == NULL) {
				select_table(tbname);
			}
			else { //옵션있을때

				column* cseek = ctop->link;
				data* datacur = cseek->datalink;
				int i, k, j;
				char tc[LINE];
				char* cmd;
				char* temp;

				printf("========== [%s] ==========\n", targetTB->tbname);
				while (cseek != NULL) {
					printf("%s\t", cseek->field);
					cseek = cseek->nextcol;
				}
				printf("\n");

				strcpy_s(tc, LINE, target_column);
				cmd = strtok_s(tc, ",", &temp);
				check_init(tbname);

				cseek = ctop->link;
				bool check = false;

				do {
					if (opts != 0) {
						strcpy_s(oper, LINE, cmd1);
					}
					if (opts == 0) {
						cmd1 = strtok_s(options, " ", &temp1);
					}
					else {
						cmd1 = strtok_s(NULL, " ", &temp1);
					}

					strcpy_s(field1, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 == NULL || (strcmp(cmd1, "=") != 0 && strcmp(cmd1, "!=") != 0)) {
						printf("query failed(wrong option1)\n");
						return;
					}
					strcpy_s(opt1, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 == NULL) {
						printf("query failed(wrong t_data1)\n");
						return;
					}
					strcpy_s(t_data1, LINE, cmd1);
					opts++;
					i = 0;
					datacur = cseek->datalink;
					while (datacur != NULL) {
						cseek = ctop->link;
						while (cseek != NULL) {
							datacur = cseek->datalink;
							k = 0;
							j = 0;
							if (strcmp(field1, cseek->field) == 0) {
								//비교 데이터가 문자형일 경우 '' 체크와 비교를 위한 변경
								char t_data_temp[LINE];
								if (strcasecmp(cseek->type, "varchar") == 0) {
									if (t_data1[0] == '\'') {
										if (t_data1[strlen(t_data1) - 1] == '\'') { //varchar형
											for (int i = 0; i < strlen(t_data1); i++) {
												t_data_temp[i] = t_data1[i + 1];
											}
											strcpy_s(t_data1, LINE, t_data_temp);
											t_data1[strlen(t_data1) - 1] = '\0';
										}
										else  // varchar형인데 '가 한개만 있는 잘못된 입력
										{
											printf("varchar type error!\n");
											return false;
										}
									}
								}

								while (datacur != NULL) {
									check = false;
									//조건에 부합하는 데이터를 찾았을 경우
									if (strcmp(opt1, "=") == 0) {
										if (strcmp(datacur->data, t_data1) == 0)
											check = true;
									}
									else if (strcmp(opt1, "!=") == 0) {
										if (strcmp(datacur->data, t_data1) != 0)
											check = true;
									}
									if (check) {
										//조건을 찾았으면
										column* c_temp;
										c_temp = ctop->link;
										while (c_temp != NULL) {
											k = 0;
											data* data_temp;
											data_temp = c_temp->datalink;
											while (k < j) {
												data_temp = data_temp->nextdata;
												k++;
											}
											data_temp->check += 1;
											c_temp = c_temp->nextcol;
										}
									}
									datacur = datacur->nextdata;
									j++;
								}
							}
							if (datacur == NULL)
								break;
							cseek = cseek->nextcol;
						}
						i++;
					}
					//컬럼 check end
					cmd1 = strtok_s(NULL, " ", &temp1);
				} while (cmd1 != NULL);
				//전체 while end

				i = 0;
				cseek = ctop->link;
				datacur = cseek->datalink;
				/* 세부 컬럼 출력 */
				while (datacur != NULL) {
					cseek = ctop->link;
					while (cseek != NULL) {
						datacur = cseek->datalink;
						k = 0;
						if (strcmp(field1, cseek->field) == 0) {
						}
						while (k < i) {
							datacur = datacur->nextdata;
							k++;
						}
						if (datacur == NULL)
							break;
						if (opts == 1) {
							if (datacur->check == 1) {
								printf("%s\t", datacur->data);
								if (cseek->nextcol == NULL) {
									printf("\n");
								}
							}
						}
						else {
							if (strcasecmp(oper, "and") == 0) {
								if (datacur->check == opts) {
									printf("%s\t", datacur->data);
									if (cseek->nextcol == NULL) {
										printf("\n");
									}
								}
							}
							else if (strcasecmp(oper, "or") == 0) {
								if (datacur->check == 1) {
									printf("%s\t", datacur->data);
									if (cseek->nextcol == NULL) {
										printf("\n");
									}
								}
							}
						}
						cseek = cseek->nextcol;
					}
					i++;
					if (datacur->nextdata == NULL)
						break;
				}
				printf("============================\n\n");
			}//옵션종료


		}
		/* 특정 column 조회 */
		else if (strcmp(target_column, "*") != 0)
		{
			column* cseek = ctop->link;
			data* datacur = cseek->datalink;
			int i = 0, j = 0, k = 0;
			char* cmd;
			char* temp;
			char* cmd0;
			char tc[LINE];
			/* 해당 컬럼이 존재하는지 확인 */
			bool target_correct = false;

			strcpy_s(tc, LINE, target_column);
			cmd = strtok_s(tc, ",", &temp);
			while (cmd != NULL) {
				target_correct = false;
				while (cseek != NULL) {
					if (strcmp(cseek->field, cmd) == 0) {
						target_correct = true;
						break;
					}

					cseek = cseek->nextcol;
				}

				cmd = strtok_s(NULL, ", ", &temp);
			}

			if (!target_correct) {
				printf("query failed!(not correctly column)\n");
				return;
			}
			/* 컬럼 확인 종료 */


			strcpy_s(tc, LINE, target_column);
			cmd = strtok_s(tc, ",", &temp);
			printf("========== [%s] ==========\n", targetTB->tbname);
			while (cmd != NULL) {

				cseek = ctop->link;
				while (cseek != NULL) {
					if (strcmp(cseek->field, cmd) == 0) {
						printf("%s\t", cseek->field);
					}
					cseek = cseek->nextcol;
				}
				cmd = strtok_s(NULL, ",", &temp);
			}
			printf("\n");


			if (options == NULL) {

				cseek = ctop->link;
				//세부 컬럼 출력
				while (datacur != NULL) {

					strcpy_s(tc, LINE, target_column);
					cmd = strtok_s(tc, ",", &temp);
					while (cmd != NULL) {

						cseek = ctop->link;
						while (cseek != NULL) {

							if (strcmp(cseek->field, cmd) == 0) {
								datacur = cseek->datalink;

								k = 0;
								while (k < i) {
									datacur = datacur->nextdata;
									k++;
								}
								printf("%s\t", datacur->data);
								break;
							}

							cseek = cseek->nextcol;

						}//cseek null시 종료
						cmd = strtok_s(NULL, ",", &temp);

					}
					if (datacur->nextdata == NULL)
						break;
					i++;
					printf("\n");
					//
				}
				printf("\n");
				printf("============================\n");
			}
			//option 추가
			else if (options != NULL) {
				check_init(tbname);

				column* cseek = ctop->link;
				data* datacur = cseek->datalink;
				int i = 0, j = 0, k = 0;
				char* cmd;
				char* temp;
				char* cmd0;
				char tc[LINE];
				bool target_correct = false;

				strcpy_s(tc, LINE, target_column);
				cmd = strtok_s(tc, ",", &temp);
				while (cmd != NULL) {
					target_correct = false;
					while (cseek != NULL) {
						if (strcmp(cseek->field, cmd) == 0) {
							target_correct = true;
							break;
						}

						cseek = cseek->nextcol;
					}

					cmd = strtok_s(NULL, ", ", &temp);
				}

				if (!target_correct) {
					printf("query failed!(not correctly column)\n");
					return;
				}

				/* 세부 컬럼 체크 */
				cseek = ctop->link;
				bool check = false;

				do {
					if (opts != 0) {
						strcpy_s(oper, LINE, cmd1);
					}
					if (opts == 0) {
						cmd1 = strtok_s(options, " ", &temp1);
					}
					else {
						cmd1 = strtok_s(NULL, " ", &temp1);
					}

					strcpy_s(field1, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 == NULL || (strcmp(cmd1, "=") != 0 && strcmp(cmd1, "!=") != 0)) {
						printf("query failed(wrong option1)\n");
						return;
					}
					strcpy_s(opt1, LINE, cmd1);
					cmd1 = strtok_s(NULL, " ", &temp1);
					if (cmd1 == NULL) {
						printf("query failed(wrong t_data1)\n");
						return;
					}
					strcpy_s(t_data1, LINE, cmd1);
					opts++;
					i = 0;
					datacur = cseek->datalink;

					while (datacur != NULL) {
						cseek = ctop->link;
						while (cseek != NULL) {

							datacur = cseek->datalink;
							k = 0;
							j = 0;
							if (strcmp(field1, cseek->field) == 0) {
								//비교 데이터가 문자형일 경우 '' 체크와 비교를 위한 변경
								char t_data_temp[LINE];
								if (strcasecmp(cseek->type, "varchar") == 0) {
									if (t_data1[0] == '\'') {
										if (t_data1[strlen(t_data1) - 1] == '\'') { //varchar형
											for (int i = 0; i < strlen(t_data1); i++) {
												t_data_temp[i] = t_data1[i + 1];
											}
											strcpy_s(t_data1, LINE, t_data_temp);
											t_data1[strlen(t_data1) - 1] = '\0';
										}
										else  // varchar형인데 '가 한개만 있는 잘못된 입력
										{
											printf("varchar type error!\n");
											return false;
										}
									}
								}

								while (datacur != NULL) {
									check = false;
									//조건에 부합하는 데이터를 찾았을 경우
									if (strcmp(opt1, "=") == 0) {
										if (strcmp(datacur->data, t_data1) == 0)
											check = true;
									}
									else if (strcmp(opt1, "!=") == 0) {
										if (strcmp(datacur->data, t_data1) != 0)
											check = true;
									}
									if (check) {
										//조건을 찾았으면
										column* c_temp;
										c_temp = ctop->link;
										while (c_temp != NULL) {
											k = 0;
											data* data_temp;
											data_temp = c_temp->datalink;
											while (k < j) {
												data_temp = data_temp->nextdata;
												k++;
											}
											data_temp->check += 1;
											c_temp = c_temp->nextcol;
										}
									}
									datacur = datacur->nextdata;
									j++;
								}
							}
							if (datacur == NULL)
								break;
							cseek = cseek->nextcol;
						}
						i++;
					}
					//컬럼 check end
					cmd1 = strtok_s(NULL, " ", &temp1);
				} while (cmd1 != NULL);
				//전체 while end



				/* 컬럼 출력 단계 */
				i = 0;
				cseek = ctop->link;
				datacur = cseek->datalink;
				bool print = false;
				print = false;

				if (datacur != NULL) {
					while (datacur->nextdata != NULL) {
						strcpy_s(tc, LINE, target_column);
						cmd = strtok_s(tc, ",", &temp);
						print = false;
						while (cmd != NULL) {

							cseek = ctop->link;
							while (cseek != NULL) {

								if (strcmp(cseek->field, cmd) == 0) {
									datacur = cseek->datalink;

									k = 0;
									while (k < i) {
										datacur = datacur->nextdata;
										k++;
									}
									if (opts == 1) {
										if (datacur->check == 1) {
											printf("%s\t", datacur->data);
											print = true;
										}
									}
									else {
										if (strcasecmp(oper, "and") == 0) {
											if (datacur->check == opts) {
												printf("%s\t", datacur->data);
												print = true;
											}
										}
										else if (strcasecmp(oper, "or") == 0) {
											if (datacur->check == 1) {
												printf("%s\t", datacur->data);
												print = true;
											}
										}
									}

								}

								cseek = cseek->nextcol;

							}//cseek null시 종료
							cmd = strtok_s(NULL, ",", &temp);

						}
						i++;
						if (print)
							printf("\n");
					}
				}

				printf("============================\n");
			}
		}


	}
	else
	{
		printf("[%s] does not exist!\n", tbname);
		return;
	}

}

//로그인 이후 명령어가 'use', 'create', 'logout', 'drop', 'exit'중 하나인 경우를 체크
int check_command(char input[]) {

	char* temp = { 0 };
	char* token = strtok_s(input, " ", &temp);

	if (strcasecmp(token, "create") == 0)
		return 1;
	else if (strcasecmp(token, "use") == 0)
		return 2;
	else if (strcasecmp(token, "show") == 0)
		return 3;
	else if (strcasecmp(token, "drop") == 0)
		return 4;
	else if (strcasecmp(token, "logout") == 0)
		return 5;
	else if (strcasecmp(token, "exit") == 0)
		return 6;
	else
		return 0;

}

int check_command2(char input[]) {
	char* temp = { 0 };
	char* token = strtok_s(input, " ", &temp);
	if (strcasecmp(token, "create") == 0)
		return 1;
	else if (strcasecmp(token, "show") == 0)
		return 2;
	else if (strcasecmp(token, "drop") == 0)
		return 3;
	else if (strcasecmp(token, "insert") == 0)
		return 4;
	else if (strcasecmp(token, "update") == 0)
		return 5;
	else if (strcasecmp(token, "delete") == 0)
		return 6;
	else if (strcasecmp(token, "select") == 0)
		return 7;
	else if (strcasecmp(token, "exit") == 0)
		return 8;
	else
		return 0;

}

// 사용중인 Database 변경
bool change_db(char dbname[]) {

	for (database* dseek = dtop->link; dseek != NULL; dseek = dseek->link) {
		if (strcmp(dseek->dbname, dbname) == 0) {

			using_db = dseek;
			ttop->link = dseek->tlink;

			if (using_db->tlink != NULL)
				ttop->link = using_db->tlink;
			else
				ttop->link = NULL;
			return 1;
			return true;
		}
	}

	return false;
}

//data의 check를 0으로 초기화하는 함수
void check_init(char tbname[]) {
	bool find = false;
	table* targetTB;
	for (table* tseek = using_db->tlink; tseek != NULL; tseek = tseek->link) {
		if (strcmp(tseek->tbname, tbname) == 0) {
			targetTB = tseek;
			find = true;
			break;
		}
	}

	if (find) {
		ctop->link = targetTB->clink;

		column* cseek = ctop->link;
		data* datacur = cseek->datalink;
		int i = 0, j = 0, k = 0;

		cseek = ctop->link;

		while (cseek != NULL) {
			datacur = cseek->datalink;

			while (datacur != NULL) {

				datacur->check = 0;

				datacur = datacur->nextdata;
			}


			cseek = cseek->nextcol;
		}


	}
	else {
		printf("there is no [%s]\n", tbname);
		return;
	}
}

char* substring(char* str, const char* word, char** context) {

	bool equal = false;
	int i = 0, j = 0, k = 0;
	char temp[MAXLINE];
	char temp1[MAXLINE];
	char* tmp;
	char* t_word;
	int len = strlen(word);


	for (i = 0; i < strlen(str); i++) {
		if (str[i] == word[0] || str[i] == word[0]+32) {

			k = i;
			equal = true;
			for (j = 0; j < strlen(word); j++) {
				if ((str[i] != word[j]) && (str[i] != word[j]+32)) {
					equal = false;
					break;

				}
				i++;
			}
		}
		if (equal)
			break;
	}

	if (equal) {
		strncpy_s(temp, MAXLINE, str, k - 1);
		tmp = &temp;
		char* tmp1;
		//k가 where 앞까지의 인덱스, word의 크기만큼 더해주고 띄어쓰기를 감안해서 1을 더해줌
		int s = 0, e = 0;
		s = k + strlen(word) + 1;
		e = strlen(str);
		char* new = (char*)malloc(MAXLINE);
		tmp1 = substr(k + strlen(word) + 1, strlen(str), str, new);
		*context = str;
		strcpy_s(temp1, MAXLINE, tmp1);
		*context += (k + len + 1);
		free(new);
		k = 0;
		return tmp;
	}
	else
	{
		return NULL;
	}
}


char* substr(int s, int e, char* str, char* new) {

	strncpy_s(new, MAXLINE, str + s, e - s + 1);
	new[e - s + 1] = 0;
	return new;
}


