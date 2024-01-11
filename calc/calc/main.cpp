#include "main.h"
#include "prefix.h"
#include "calculate.h"


int main() {
	char* formula = NULL;

	printf("수식을 입력하세요.\n");
	formula = getString();

	printf("입력한 수식 : %s\n", formula);

	int result = 0;// 0:성공, 1:잘못된 수식, 2:괄호 미완성

	result = toPrefix(formula);
	
	if (result == 0) printf("변경된 수식 : %s\n", formula);
	else			 printf("잘못된 수식입니다.\n");
	
	double calc_result = calculate(formula);
	printf("계산결과 : %.3f\n", calc_result);

	free(formula);

	return 0;
}


char* getString() {
	char temp[1000] = { '\0', };
	char ch;
	int index = 0;

	do {
		ch = getchar();
		if (ch != '\n') temp[index++] = ch;
	} while (ch != '\n');

	char* formula = (char*)malloc(sizeof(char) * (index + 1) * 2);
	strncpy_s(formula, index + 1, temp, index);
	return formula;

}
