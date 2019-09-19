#include "HelloServiceServerStub.h"

#include "Channel.h"
#include "HelloService.h"

#include <pthread.h>
#include <stdio.h>
#include <string>
#include <string.h>

static void *hello_server_routine(void *arg)
{
	Channel channel("/hello");
	Channel channel_recv("/hello/recv");
	while (true) {
		char *recv = channel.receive();
		if (recv == NULL)
		{
			continue;
		}

		HelloService service;
		// Unmarshalling
		if (recv[0] == 'p')
		{
			service.print(recv + 1);
		}
		else
		{
			int ret = service.add(recv[1] - '0', recv[3] - '0');
			// Marshalling
			char *ret_str = (char *)malloc(sizeof(char) * 10);
			sprintf(ret_str, "%d", ret);
			channel_recv.send(ret_str);
		}		
	}
	return NULL;
}

void HelloServiceServerStub::run(void)
{
	pthread_t pid;
	pthread_create(&pid, NULL, hello_server_routine, NULL);
	printf("helloxx_server start\n");
	pthread_join(pid, NULL);
}
