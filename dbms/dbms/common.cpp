#include "common.h"


void getString(char* str) {
	char ch = 0;
	int index = 0;
	printf("ÀÔ·Â >>> ");
	do {
		ch = getchar();
		if (ch != '\n') str[index++] = ch;
	} while (ch != '\n');
}

char* trim_left(char* str) {
	while (*str) {
		if (isspace(*str)) {
			str++;
		}
		else {
			break;
		}
	}
	return str;
}

char* trim_right(char* str) {
	int len = (int)strlen(str) - 1;

	while (len >= 0) {
		if (isspace(*(str + len))) {
			len--;
		}
		else {
			break;
		}
	}
	*(str + ++len) = '\0';
	return str;
}

char* trim(char* str) {
	return trim_left(trim_right(str));
}