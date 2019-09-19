#pragma once

class HelloServiceInterface
{
public:
	virtual void print(char *str) = 0;
	virtual int add(const int a, const int b) = 0;
};
