#include "HelloServiceServerStub.h"

#include "Channel.h"
#include "HelloService.h"

#include <pthread.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "hello.pb.h"

static void *hello_server_routine(void *arg)
{
	Channel channel("/hello");
	Channel channel_recv("/hello/recv");
	while (true) {
		HelloService service;
		int32_t a, b;

		{
			AddRequest addReq = AddRequest_init_zero;
			uint8_t *recv = (uint8_t *)channel.receive(128);
			if (recv == NULL) {
				continue;
			}
			pb_istream_t stream = pb_istream_from_buffer(recv, 128);
			pb_decode(&stream, AddRequest_fields, &addReq);
			a = addReq.param1;
			b = addReq.param2;
		}

		int ret = service.add(a, b);

		{
			uint8_t buffer[128];
			pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
			AddResponse addRes = AddResponse_init_zero;
			addRes.sum = ret;
			pb_encode(&stream, AddResponse_fields, &addRes);
			channel_recv.send((char *)buffer, stream.bytes_written);
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
