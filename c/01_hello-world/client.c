/// "Hello, world!" client.

#include <zmq.h>
#include <assert.h>
#include <stdio.h>

int main(void)
{
	enum {
		NR_ITERATIONS = 10,
		BUF_SIZE = 16
	};
	char buf[BUF_SIZE] = {};
	char req[] = "Hello";
	void *context;
	void *requester;
	int rc;
	int i;

	context = zmq_ctx_new();
	assert(context != NULL);

	requester = zmq_socket(context, ZMQ_REQ);
	assert(requester != NULL);

	rc = zmq_connect(requester, "tcp://localhost:5555");
	assert(rc == 0);

	for (i = 0; i < NR_ITERATIONS; ++i) {
		rc = zmq_send(requester, req, sizeof(req) - 1, 0);
		assert(rc == sizeof(req) - 1);
		rc = zmq_recv(requester, buf, BUF_SIZE, 0);
		assert(rc != -1);
		fprintf(stderr, "[%d] Received %d bytes\n", i, rc);
	}
	rc = zmq_close(requester);
	assert(rc == 0);
	rc = zmq_ctx_term(context);
	assert(rc == 0);

	return 0;
}
