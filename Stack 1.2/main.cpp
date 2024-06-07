#define _CRT_SECURE_NO_WARNINGS
#define N 20

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "stack.h"

int prior(double oper);
void handlingOper(double oper, double* stack, int* top, char* postfixNotation, int* cnt);

int main() {
	setlocale(LC_ALL, "Rus");
	FILE* fp;
	fp = fopen("arithmetic_expressions.txt", "r");
	if (fp == NULL) {
		printf("Не удалось открыть файл");
		return 0;
	}

	double stack[N] = { NULL };
	int top = -1;
	char expression[30] = { NULL };

	while (fgets(expression, 30, fp) != NULL) {

		printf("Исходное выражение: %s\n", expression);

		int isCorrect = 1;
		for (int i = 0; i < strlen(expression); i++) {
			if (expression[i] == '(')
				if (!(is_full(top)))
					push(stack, &top, 1.0);
				else {
					printf("Вызвано переполнение стека! Строка не обработана.\n");
					clear(&top);
					isCorrect = 0;
					break;
				}
			else if (expression[i] == ')') {
				if (!(is_empty(top)))
					pop(stack, &top);
				else {
					printf("Вызвана операция удаления из пустого стека. Выражение некорректно.\n");
					isCorrect = 0;
					break;
				}
			}
		}
		if (!is_empty(top)) {
			printf("Выражение некорректно\n");
			isCorrect = 0;
		}

		if (isCorrect) {
			printf("Выражение корректно\n");
			clear(&top);

			char* postfixNotation = (char*)calloc(N, sizeof(char));
			int cnt = 0;
			char operSymbols[] = { '(', ')', '+', '-', '*', '/' };
			for (int i = 0; i < strlen(expression); i++) {
				if ((int(expression[i]) >= 48) && (int(expression[i]) <= 57))
					postfixNotation[cnt++] = expression[i];
				else if (expression[i] == '(')
					push(stack, &top, 1.0);
				else if (expression[i] == ')') {
					while (!is_empty(top) && peek(stack, top) != 1.0) {
						if (peek(stack, top) != 1.0) {
							char symbol = operSymbols[int(peek(stack, top))];
							postfixNotation[cnt++] = symbol;
							pop(stack, &top);
						}
						else {
							pop(stack, &top);
							break;
						}
					}
				}
				else {
					switch (expression[i]) {
					case '+':
						handlingOper(2.0, stack, &top, postfixNotation, &cnt);
						break;
					case '-':
						handlingOper(3.0, stack, &top, postfixNotation, &cnt);
						break;
					case '*':
						handlingOper(4.0, stack, &top, postfixNotation, &cnt);
						break;
					case '/':
						handlingOper(5.0, stack, &top, postfixNotation, &cnt);
						break;
					}
				}
			}

			while (!is_empty(top)) {
				if (peek(stack, top) == 1.0) {
					pop(stack, &top);
					continue;
				}
				char symbol = operSymbols[int(peek(stack, top))];
				postfixNotation[cnt++] = symbol;
				pop(stack, &top);
			}
			clear(&top);

			printf("Результат в постфиксной записи: ");
			postfixNotation[cnt++] = '\0';
			for (int i = 0; i < sizeof(postfixNotation) / sizeof(char); i++)
				printf("%c", postfixNotation[i]);

			printf("\n");


			for (int i = 0; i < strlen(postfixNotation); i++) {
				if ((int(postfixNotation[i]) >= 48) && (int(postfixNotation[i]) <= 57)) {
					char string[2] = { NULL };
					string[0] = postfixNotation[i];
					string[1] = '\0';
					push(stack, &top, atof(string));

				}
				else {
					double result = -1.0;
					switch (postfixNotation[i]) {
					case '+':
						result = pop(stack, &top) + pop(stack, &top);
						push(stack, &top, result);
						break;
					case '-':
						result = pop(stack, &top) - pop(stack, &top);
						push(stack, &top, result);
						break;
					case '*':
						result = pop(stack, &top) * pop(stack, &top);
						push(stack, &top, result);
						break;
					case '/':
						result = 1/pop(stack, &top) * pop(stack, &top);
						push(stack, &top, result);
						break;
					}
				}
			}
			printf("Результат вычисления выражения в постфиксной записи: %g", peek(stack, top));
			printf("\n-----------------\n");
			free(postfixNotation);

		}
		clear(&top);
	}
}

int prior(double oper) {
	switch (int(oper)) {
	case 1:
		return 0;
	case 0:
		return 1;
	case 2: // +
	case 3: // -
		return 2;
	case 4: // *
	case 5: // "/"
		return 3;
	}
}

void handlingOper(double oper, double* stack, int* top, char* postfixNotation, int* cnt) {
	char operSymbols[] = { '(', ')', '+', '-', '*', '/' };
	double operNew = prior(oper);
	if (is_empty(*top))
		push(stack, top, oper);
	else if (prior(operNew) > prior(peek(stack, *top)))
		push(stack, top, oper);
	else {
		while (!is_empty(*top)) {
			if (prior(peek(stack, *top)) >= operNew) {
				char symbol = operSymbols[int(peek(stack, *top))];
				postfixNotation[(*cnt)++] = symbol;
				pop(stack, top);
			}
			else
				break;
		}
		push(stack, top, oper);
	}
}