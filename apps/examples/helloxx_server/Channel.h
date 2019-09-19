#pragma once

#include <mqueue.h>

class Channel
{
public:
	Channel(char *name);
	~Channel();

	void send(const char *str);
	char *receive(void);
private:
	mqd_t mfd;
};