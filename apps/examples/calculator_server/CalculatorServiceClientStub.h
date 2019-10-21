#pragma once

#include "CalculatorServiceInterface.h"

class CalculatorServiceClientStub : public CalculatorServiceInterface
{
public:
	void print(const char *str) override;
	int add(const int a, const int b) override;
	int square(const int a) override;
};
