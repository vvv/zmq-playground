/// "Hello, world!" server.

#include <zmq.h>
#include <assert.h>
#include <unistd.h>  // sleep
#include <stdio.h>

int main(void)
{
	enum { BUF_SIZE = 16 };
	char buf[BUF_SIZE] = {};
	char reply[] = "world";
	void *context;
	void *responder;
	int rc;

	context = zmq_ctx_new();
	assert(context != NULL);

	responder = zmq_socket(context, ZMQ_REP);
	assert(responder != NULL);

	rc = zmq_bind(responder, "tcp://*:5555");
	assert(rc == 0);

	while (1) {
		rc = zmq_recv(responder, buf, BUF_SIZE, 0);
		assert(rc != -1);
		fprintf(stderr, "Received %d bytes\n", rc);
		sleep(1);  // do some "work"
		rc = zmq_send(responder, reply, sizeof(reply) - 1, 0);
		assert(rc == sizeof(reply) - 1);
	}
	return 0;
}
