#pragma once

#include <mqueue.h>

class Channel
{
public:
	Channel(const char *name);
	~Channel();

	int send(const char *str, size_t n);
	int receive(char *buf, size_t n);
private:
	mqd_t mfd;
};