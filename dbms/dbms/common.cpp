#include "common.h"

char* getString() {
	char temp[1000] = { '\0', };
	char ch;
	int index = 0;
	printf("�Է� >>> ");
	do {
		ch = getchar();
		if (ch != '\n') temp[index++] = ch;
	} while (ch != '\n');

	char* query = (char*)malloc(sizeof(char) * index);
	strncpy_s(query, index+1, temp, index);
	return query;
}