#define _CRT_SECURE_NO_WARNINGS
#define N 10

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
void push(double* stack, int* top, double value) {
	(*top)++;
	stack[*top] = value;
}

double pop(double* stack, int* top) {
	(*top)--;
	return stack[*top + 1];
}

double peek(double* stack, int top) {
	return stack[top];
}

int is_empty(int top) {
	if (top > -1)
		return 0;
	else
		return 1;
}

int is_full(int top) {
	if (top == N-1)
		return 1;
	else
		return 0;
}

int size(int top) {
	return top + 1;
}

void clear(int* top) {
	*top = -1;
}
