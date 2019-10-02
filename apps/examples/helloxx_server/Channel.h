#pragma once

#include <mqueue.h>

class Channel
{
public:
	Channel(const char *name);
	~Channel();

	void send(const char *str, int n);
	char *receive(int n);
private:
	mqd_t mfd;
};