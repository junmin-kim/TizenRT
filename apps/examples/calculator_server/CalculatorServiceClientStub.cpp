#include "CalculatorServiceClientStub.h"
#include "Channel.h"
#include <string>
#include <string.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "calculator.pb.h"

bool encode_unionmessage(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
{
	pb_field_iter_t iter;

	if (!pb_field_iter_begin(&iter, CalculatorRequestMsg_fields, message))
		return false;

	do {
		if (iter.submsg_desc == messagetype) {
			/* This is our field, encode the message using it. */
			if (!pb_encode_tag_for_field(stream, &iter))
				return false;

			return pb_encode_submessage(stream, messagetype, message);
		}
	} while (pb_field_iter_next(&iter));

	/* Didn't find the field for messagetype */
	return false;
}

static bool write_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
	return pb_encode_tag_for_field(stream, field) &&
		   pb_encode_string(stream, (const pb_byte_t *)*arg, strlen((const char *)*arg));
}

void CalculatorServiceClientStub::print(const char *a)
{
	uint8_t buffer[128];
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

	PrintRequest msg = PrintRequest_init_zero;
	msg.param1.funcs.encode = &write_string;
	msg.param1.arg = (void *)a;

	bool status = encode_unionmessage(&stream, PrintRequest_fields, &msg);
	if (status == false) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
	}

	Channel ch_req("/calculator/req");
	ch_req.send((const char *)buffer, stream.bytes_written);
}

int CalculatorServiceClientStub::add(const int a, const int b)
{
	pb_byte_t buffer[128];
	int ret = 0;
	{
		pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

		AddRequest req = {a, b};

		bool status = encode_unionmessage(&stream, AddRequest_fields, &req);
		if (status == false) {
			printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		}

		Channel ch_req("/calculator/req");
		ch_req.send((const char *)buffer, stream.bytes_written);
	}
	{
		Channel ch_res("/calculator/res");
		int recv = ch_res.receive((char *)buffer, 128);

		pb_istream_t stream = pb_istream_from_buffer(buffer, recv);
		AddResponse res = AddResponse_init_zero;

		pb_decode(&stream, AddResponse_fields, &res);
		ret = res.value;
	}
	return ret;
}

int CalculatorServiceClientStub::square(const int a)
{
	pb_byte_t buffer[128];
	int ret = 0;
	{
		pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

		AddRequest req = {a};

		bool status = encode_unionmessage(&stream, SquareRequest_fields, &req);
		if (status == false) {
			printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		}

		Channel ch_req("/calculator/req");
		ch_req.send((const char *)buffer, stream.bytes_written);
	}
	{
		Channel ch_res("/calculator/res");
		int recv = ch_res.receive((char *)buffer, 128);

		pb_istream_t stream = pb_istream_from_buffer(buffer, recv);
		SquareResponse res = SquareResponse_init_zero;

		pb_decode(&stream, SquareResponse_fields, &res);
		ret = res.value;
	}

	return ret;
}