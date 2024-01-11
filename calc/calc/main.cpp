#include "main.h"
#include "prefix.h"
#include "calculate.h"


int main() {
	char* formula = NULL;

	printf("������ �Է��ϼ���.\n");
	formula = getString();

	printf("�Է��� ���� : %s\n", formula);

	int result = 0;// 0:����, 1:�߸��� ����, 2:��ȣ �̿ϼ�

	result = toPrefix(formula);
	
	if (result == 0) printf("����� ���� : %s\n", formula);
	else			 printf("�߸��� �����Դϴ�.\n");
	
	double calc_result = calculate(formula);
	printf("����� : %.3f\n", calc_result);

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
