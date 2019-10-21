#include "CalculatorServiceServerStub.h"

#include "Channel.h"
#include "CalculatorService.h"

#include <pthread.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "calculator.pb.h"

const pb_msgdesc_t* decode_unionmessage_type(pb_istream_t *stream)
{
    pb_wire_type_t wire_type;
    uint32_t tag;
    bool eof;

    while (pb_decode_tag(stream, &wire_type, &tag, &eof))
    {
        if (wire_type == PB_WT_STRING)
        {
            pb_field_iter_t iter;
            if (pb_field_iter_begin(&iter, CalculatorRequestMsg_fields, NULL) &&
                pb_field_iter_find(&iter, tag))
            {
                /* Found our field. */
                return iter.submsg_desc;
            }
        }
        
        /* Wasn't our field.. */
        pb_skip_field(stream, wire_type);
    }
    
    return NULL;
}

bool decode_unionmessage_contents(pb_istream_t *stream, const pb_msgdesc_t *messagetype, void *dest_struct)
{
    pb_istream_t substream;
    bool status;
    if (!pb_make_string_substream(stream, &substream))
        return false;
    
    status = pb_decode(&substream, messagetype, dest_struct);
    pb_close_string_substream(stream, &substream);
    return status;
}

static bool read_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    size_t len = stream->bytes_left;
    pb_byte_t *str = (pb_byte_t *)malloc((len + 1) * sizeof(pb_byte_t));
    
    if (!pb_read(stream, str, len))
        return false;

	str[len] = '\0';
    *arg = str;
    return true;
}

static void *calculator_server_routine(void *arg)
{
	Channel ch_req("/calculator/req");
	Channel ch_res("/calculator/res");
	while (true) {
		CalculatorService service;
		pb_byte_t buf_req[128];
		int recv = ch_req.receive((char *)buf_req, 128);
		if (recv == NULL) {
			continue;
		}
		pb_istream_t istream = pb_istream_from_buffer(buf_req, recv);
		const pb_msgdesc_t *type = decode_unionmessage_type(&istream);

		if (type == PrintRequest_fields) {
			PrintRequest req = PrintRequest_init_zero;
			req.param1.funcs.decode = &read_string;
			decode_unionmessage_contents(&istream, PrintRequest_fields, &req);
			service.print((const char *)req.param1.arg);
			if (req.param1.arg) {
				free(req.param1.arg);
			}
		} else if (type == AddRequest_fields) {
			AddRequest req = AddRequest_init_zero;
			AddResponse res = AddResponse_init_zero;
			decode_unionmessage_contents(&istream, AddRequest_fields, &req);
			res.value = service.add(req.param1, req.param2);

			pb_byte_t buf_res[128];
			pb_ostream_t ostream = pb_ostream_from_buffer(buf_res, sizeof(buf_res));
			pb_encode(&ostream, AddResponse_fields, &res);
			ch_res.send((const char *)buf_res, ostream.bytes_written);
		} else if (type == SquareRequest_fields) {
			SquareRequest req = SquareRequest_init_zero;
			SquareResponse res = SquareResponse_init_zero;
			decode_unionmessage_contents(&istream, SquareRequest_fields, &req);
			res.value = service.square(req.param1);

			pb_byte_t buf_res[128];
			pb_ostream_t ostream = pb_ostream_from_buffer(buf_res, sizeof(buf_res));
			pb_encode(&ostream, SquareResponse_fields, &res);
			ch_res.send((const char *)buf_res, ostream.bytes_written);
		}
	}
	return NULL;
}

void CalculatorServiceServerStub::run(void)
{
	pthread_t pid;
	pthread_create(&pid, NULL, calculator_server_routine, NULL);
	printf("calculator_server start\n");
	pthread_join(pid, NULL);
}
