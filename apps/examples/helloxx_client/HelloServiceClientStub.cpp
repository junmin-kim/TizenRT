#include "HelloServiceClientStub.h"
#include "../helloxx_server/Channel.h"
#include <string>
#include <string.h>

void HelloServiceClientStub::print(char *a)
{
	// Marshalling
	std::string str = 'p' + str;

	Channel channel("/hello");
	channel.send(str.c_str());
}

int HelloServiceClientStub::add(const int a, const int b)
{
	std::string str = 'a' + std::to_string(a) + '+' + std::to_string(b);
	Channel channel_send("/hello");
	Channel channel_recv("/hello/recv");
	channel_send.send(str.c_str());
	char *recv = channel_recv.receive();

	// Unmarshalling
	int ret = 0;
	ret = std::stoi(std::string(recv));
	return ret;
}
