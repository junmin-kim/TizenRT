#pragma once
#include "HelloServiceInterface.h"

class HelloService : public HelloServiceInterface
{
public:
	void print(char *str) override;
	int add(const int a, const int b) override;
};
