#include "prefix.h"

char stack[100];
int pointer = 0;


int toPrefix(char* formula) {
	int result = 0; // 0:성공, 1:잘못된 수식, 2:괄호 미완성

	int bf_idx = 0, af_idx = 0, stack_idx = 0;
	char temp[100] = { '\0', }; // 사이즈 임시
	char pre = NULL;
	
	while (formula[bf_idx] != NULL) {
		if (bf_idx > 0) pre = formula[bf_idx - 1];
		char ch = formula[bf_idx];

		if ('0' <= ch && ch <= '9') {
			temp[af_idx++] = ch;
		}
		else {
			if (isEmpty()) {
				stack_push(ch);
				temp[af_idx++] = ' ';
			}
			else {
				if (ch == '+' || ch == '-') {
					if (check_priority(ch) == -1) {
						temp[af_idx++] = stack_pop();
						stack_push(ch);
						temp[af_idx++] = ' ';
					}
					else {
						stack_push(ch);
						temp[af_idx++] = ' ';
					}
				}
				else if (ch == '*' || ch == '/') {
					if (check_priority(ch) == -1) {
						temp[af_idx++] = stack_pop();
						stack_push(ch);
						temp[af_idx++] = ' ';
					}
					else {
						stack_push(ch);
						temp[af_idx++] = ' ';
					}
				}
				else if (ch == '(' || ch == ')') {
					if (ch == '(') {
						stack_push(ch);
						temp[af_idx++] = ' ';
					}
					else {
						while (stack[pointer - 1] != '(') {
							temp[af_idx++] = stack_pop();
						}
						stack_pop();
					}
				}
				else {
					result = 1;
					break;
				}
			}
		}

		++bf_idx;
	}

	if (pointer > 0) {
		do {
			temp[af_idx++] = stack_pop();
		} while (pointer > 0);
	}
	const int str_size = strlen(temp);
	strncpy_s(formula, str_size + 1, temp, str_size);
	
	return result;
}

bool isEmpty() {
	if (pointer < 1) return true;
	return false;
}

void stack_push(char oper) {
	stack[pointer++] = oper;
}

char stack_pop() {
	char oper = stack[--pointer];
	stack[pointer] = NULL;
	return oper;
}

int check_priority(char oper) {
	int prior = -1; //1:우선순위, 1:ㅇㅇ, -1:후순위
	char pre_oper = stack[pointer - 1];
	if (oper == '+' || oper == '-') {
		if (pre_oper == '(' || pre_oper == ')') {
			prior = 1;
		}
		else if (pre_oper == '+' || pre_oper == '-') {
			prior = -1;
		}
		else {
			prior = -1;
		}
	}
	else if (oper == '*' || oper == '/') {
		if (pre_oper == '+' || pre_oper == '-') {
			prior = 1;
		}

	}
	printf("우선순위 : %d (연산자 : %c, %c)\n", prior, pre_oper, oper);
	return prior;
}