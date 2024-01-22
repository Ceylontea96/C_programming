#include "common.h"

char* getString() {
	char temp[1000] = { '\0', };
	char ch;
	int index = 0;
	printf("ÀÔ·Â >>> ");
	do {
		ch = getchar();
		if (ch != '\n') temp[index++] = ch;
	} while (ch != '\n');

	char* query = (char*)malloc(sizeof(char) * index);
	strncpy_s(query, index+1, temp, index);
	return query;
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