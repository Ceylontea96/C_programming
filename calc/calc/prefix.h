#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int toPrefix(char* formula);
bool isEmpty();
void stack_push(char oper);
char stack_pop();
int check_priority(char oper);