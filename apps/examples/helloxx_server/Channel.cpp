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

void Channel::send(const char *str, int n)
{
	if (mfd == NULL) {
		printf("mq /hello open fail\n");
		return;
	}

	char *buffer = (char *)malloc(sizeof(char) * n);
	memcpy(buffer, str, n);

	mq_send(mfd, buffer, n, 100);
	free(buffer);
}

char *Channel::receive(int n)
{
	char *buffer = (char *)malloc(sizeof(char) * n);

	if ((mq_receive(mfd, buffer, n, NULL)) == -1) {
		printf("mq_recevice fail\n");
		return NULL;
	}

	return buffer;
}
