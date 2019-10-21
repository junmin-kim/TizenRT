#include "CalculatorService.h"
#include <stdio.h>

void CalculatorService::print(const char *str)
{
	printf("%s", str);
}

int CalculatorService::add(const int a, const int b)
{
	return a + b;
}

int CalculatorService::square(const int a)
{
	return a * a;
}
