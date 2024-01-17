#ifndef _USER_
#define _USER_ 
#include "common.h"
#include "structs.h"

extern user* login_user;
extern user* userTop;

user* getUserByName(char* userName);
bool isDupId(char* id);
bool checkPw(char* id, char* pw);
void create_user(char* id, char* pw, int right);
database* delete_user(char* id);
void showUsers();
#endif

