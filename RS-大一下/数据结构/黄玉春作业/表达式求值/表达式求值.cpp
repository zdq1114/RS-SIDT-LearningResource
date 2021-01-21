#include<stdlib.h>
#include<stdio.h>
#include<iostream>
using namespace std;
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
typedef char SElemType;

typedef struct {
	SElemType *base;
	SElemType *top;
	int stacksize;
}SqStack;

void InitStack(SqStack &S) {
	S.base = new SElemType[STACK_INIT_SIZE];
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
}

void DestoyStack(SqStack &S) {
	if (S.base)delete[] S.base;
}

bool StackEmpty(SqStack S) {
	if (S.base == S.top)return true;
	else return false;
}

SElemType GetTop(SqStack S) {
	if (!StackEmpty(S)) {
		return *(S.top - 1);
	}
}

void Push(SqStack &S, SElemType e) {
	if (S.top - S.base >= S.stacksize) {
		S.base = (SElemType*)realloc(S.base, S.stacksize + STACKINCREMENT);
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
}

void Pop(SqStack &S, SElemType &e) {
	e = *--S.top;
}

bool In(char c) {
	switch (c) {
	case'+':
	case'-':
	case'*':
	case'/':
	case'(':
	case')':
	case'#':
		return true;
	default:
		return false;
	}
}

char Precede(char a, char b) {
	switch (a) {
	case '+':
		switch (b) {
		case '+':return '>';
		case '-':return '>';
		case '*':return '<';
		case '/':return'<';
		case '(':return'<';
		case ')':return'>';
		case '#':return'>';
		}
	case '-':
		switch (b) {
		case '+':return '>';
		case '-':return '>';
		case '*':return '<';
		case '/':return'<';
		case '(':return'<';
		case ')':return'>';
		case '#':return'>';
		}
	case '*':
		switch (b) {
		case '+':return '>';
		case '-':return '>';
		case '*':return '>';
		case '/':return'>';
		case '(':return'<';
		case ')':return'>';
		case '#':return'>';
		}
	case '/':
		switch (b) {
		case '+':return '>';
		case '-':return '>';
		case '*':return '>';
		case '/':return'>';
		case '(':return'<';
		case ')':return'>';
		case '#':return'>';
		}
	case '(':
		switch (b) {
		case '+':return '<';
		case '-':return '<';
		case '*':return '<';
		case '/':return'<';
		case '(':return'<';
		case ')':return'=';
		}
	case ')':
		switch (b) {
		case '+':return '>';
		case '-':return '>';
		case '*':return '>';
		case '/':return'>';
		case ')':return'>';
		case '#':return'>';
		}
	case '#':
		switch (b) {
		case '+':return '<';
		case '-':return '<';
		case '*':return '<';
		case '/':return'<';
		case '(':return'<';
		case '#':return'=';
		}
	}
}

int Operate(SElemType a, SElemType theta, SElemType b) {
	int result = 0;
	switch (theta) {
	case'+':result = a + b; break;
	case'-':result = a - b; break;
	case'*':result = a * b; break;
	case'/':result = a / b; break;
	}
	return result;
}

int EvaluateExpression() {
	SqStack OPTR, OPND;
	InitStack(OPTR);
	Push(OPTR, '#');
	InitStack(OPND);
	SElemType c = getchar();
	while (c != '#' || GetTop(OPTR) != '#') {
		if (!In(c)) {
			char temp[STACK_INIT_SIZE] = { 0 };
			for (int i = 0; !In(c); i++) {
				temp[i] = c;
				c = getchar();
			}
			int d = atoi(temp);
			Push(OPND, d);
		}
		else {
			switch (Precede(GetTop(OPTR), c)) {
			case '<':
				Push(OPTR, c);
				c = getchar();
				break;
			case'=':
				Pop(OPTR, c);
				c = getchar();
				break;
			case'>':
				SElemType theta, a, b;
				Pop(OPTR, theta);
				Pop(OPND, b);
				Pop(OPND, a);
				Push(OPND, Operate(a, theta, b));
				break;
			}
		}
	}
	int result = GetTop(OPND);
	DestoyStack(OPND);
	DestoyStack(OPTR);
	return result;
}

void main() {
	cout << "Please enter an operand(end with #):" << endl;
	cout << EvaluateExpression() << endl;
}