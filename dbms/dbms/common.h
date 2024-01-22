#ifndef _COMMON_
#define _COMMON_ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 1000

char* getString();

char* trim_left(char* str);
char* trim_right(char* str);
char* trim(char* str);
#endif
