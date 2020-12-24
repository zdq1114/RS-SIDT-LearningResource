
#include <stdio.h>
#include <memory.h>

typedef struct tagMyDate {
	unsigned int year, month, date;
}MyDate, *MyDatePtr;

const int DaysInMonth[] = {
	0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
	0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

bool IsLeapYear(unsigned int year) {
	return (((0 == year % 4) && (0 != year % 100)) || (0 == year % 400)) ? true : false;
}

int GetSign(int value) {
	if (value < 0) return -1;
	else if (value > 0) return 1;
	return 0;
}

int CompareDate(MyDatePtr t1, MyDatePtr t2) {
	int symbol = GetSign(t1->year - t2->year);
	if (symbol == 0) {
		symbol = GetSign(t1->month - t2->month);
		if (symbol == 0) {
			symbol = GetSign(t1->date - t2->date);
		}
	}
	return symbol;
}

MyDatePtr InputNewDate() {
	MyDatePtr t = new MyDate;
	printf("Please enter a date(xxxx/xx/xx): ");
	scanf("%d/%d/%d", &t->year, &t->month, &t->date);
	return t;
}

int GetDaysFromTheYear(MyDatePtr t) {
	const int* pDays = (IsLeapYear(t->year)) ? DaysInMonth + 13 : DaysInMonth;

	int days = 0;
	for (int i = 1; i < t->month; i++) {
		days += pDays[i];
	}
	days += t->date;

	return days - 1;
}

int GetDifferentDays(MyDatePtr t1, MyDatePtr t2) {
	int sign = CompareDate(t1, t2);
	if (sign == 0) return 0;

	MyDatePtr ref, tar;
	if (sign < 0) { ref = t1; tar = t2; }
	else          { ref = t2; tar = t1; }

	int days = GetDaysFromTheYear(tar) - GetDaysFromTheYear(ref);
	for (int i = ref->year; i < tar->year; i++) {
		int n = (IsLeapYear(i)) ? 366 : 365;
		days += n;
	}

	return sign*days;
}

void main() {
	printf("Compare two input date.\n");
	MyDatePtr t1 = InputNewDate();
	MyDatePtr t2 = InputNewDate();
	printf("%d\n", GetDifferentDays(t1, t2));
	if (t1) delete[] t1;
	if (t2) delete[] t2;
}