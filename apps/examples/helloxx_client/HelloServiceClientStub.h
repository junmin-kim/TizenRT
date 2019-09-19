#pragma once

#include "../helloxx_server/HelloServiceInterface.h"

class HelloServiceClientStub : public HelloServiceInterface
{
public:
	void print(char *str) override;
	int add(const int a, const int b) override;
};
