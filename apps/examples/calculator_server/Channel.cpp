#include "Channel.h"

#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static constexpr int msgsize = 32;

Channel::Channel(const char *name)
{
	struct mq_attr attr;
	attr.mq_maxmsg = 20;
	attr.mq_msgsize = msgsize;
	attr.mq_flags = 0;
	mfd = mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
}

Channel::~Channel()
{
	if (mfd != NULL) {
		mq_close(mfd);
	}
}

int Channel::send(const char *buf, size_t n)
{
	if (mfd == NULL) {
		printf("mq open fail\n");
		return -1;
	}

	return mq_send(mfd, buf, n, 100);
}

int Channel::receive(char *buf, size_t n)
{
	int ret = mq_receive(mfd, buf, n, NULL);
	if ((ret) == -1) {
		printf("mq_receive fail\n");
		return -1;
	}

	return ret;
}
