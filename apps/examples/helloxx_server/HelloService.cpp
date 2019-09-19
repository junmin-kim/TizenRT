#include "HelloService.h"
#include "stdio.h"

void HelloService::print(char *str)
{
	printf("%s\n", str);
}

int HelloService::add(const int a, const int b)
{
	return a + b;
}