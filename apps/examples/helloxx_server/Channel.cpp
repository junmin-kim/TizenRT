#include "Channel.h"

#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static constexpr int msgsize = 11;

Channel::Channel(char *name)
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

void Channel::send(const char *str)
{
	if (mfd == NULL) {
		printf("mq /hello open fail\n");
		return;
	}
	char *buffer = strdup(str);

	mq_send(mfd, buffer, msgsize, 100);
	free(buffer);
}

char *Channel::receive(void)
{
	char buffer[msgsize];

	if ((mq_receive(mfd, buffer, msgsize, NULL)) == -1) {
		printf("mq_recevice fail\n");
		return NULL;
	}
	return strdup(buffer);
}
