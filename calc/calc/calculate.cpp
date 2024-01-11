#include "calculate.h"

double i_stack[100] = { '\0', };
int i_pointer = 0;

double calculate(char* formula) {
	i_pointer = 0;
	int index = 0, numIdx = 0, preNum = 0;
	char numArr[100] = { '\0', };
	while (formula[index] != NULL) {
		char ch = formula[index];

		if ('0' <= ch && ch <= '9') {
			if (preNum == 0)	preNum = ch - '0';
			else				preNum = preNum * 10 + (ch - '0');
		}
		else if (ch == ' ') {
			if (preNum != 0) {
				i_stack_push((double)preNum);
				preNum = 0;
			}
		}
		else {
			if (preNum != 0) {
				i_stack_push((double)preNum);
				preNum = 0;
			}
			double num2 = i_stack_pop();
			double num1 = i_stack_pop();
			double result = -1;
			if (ch == '+')		result = num1 + num2;
			else if (ch == '-') result = num1 - num2;
			else if (ch == '*') result = num1 * num2;
			else if (ch == '/') result = num1 / num2;
			i_stack_push(result);
		}
		++index;
	}
	return i_stack_pop();
}

void i_stack_push(double num) {
	i_stack[i_pointer++] = num;
}

double i_stack_pop() {
	double num = i_stack[--i_pointer];
	i_stack[i_pointer] = NULL;
	return num;
}