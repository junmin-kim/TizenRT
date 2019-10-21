#pragma once

class CalculatorServiceInterface
{
public:
	virtual void print(const char *str) = 0;
	virtual int add(const int a, const int b) = 0;
	virtual int square(const int a) = 0;
};
