#include "HelloServiceClientStub.h"
#include "Channel.h"
#include <string>
#include <string.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "hello.pb.h"

void HelloServiceClientStub::print(char *a)
{
}

int HelloServiceClientStub::add(const int a, const int b)
{
	int ret = 0;
	{
		uint8_t buffer[128];
		pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
		AddRequest addReq = AddRequest_init_zero;

		printf("clinentstub::add %d %d\n", a, b);
		addReq.param1 = a;
		addReq.param2 = b;

		bool status = pb_encode(&stream, AddRequest_fields, &addReq);
		if (status == false)
		{
            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		}

		Channel channel("/hello");
		channel.send((const char *)buffer, stream.bytes_written);
	}

	{
		Channel channel_recv("/hello/recv");
		char *recv = channel_recv.receive(128);

		pb_istream_t stream = pb_istream_from_buffer((uint8_t *)recv, 128);
		AddResponse addRes = AddResponse_init_zero;

		pb_decode(&stream, AddResponse_fields, &addRes);
		ret = addRes.sum;
	}
	return ret;
}
