/// Task sink.
/// Binds PULL socket to tcp://localhost:5558.
/// Collects results from workers via that socket.

#include <zmq.h>
#include <assert.h>
#include <stdio.h>
#include "helpers.h"

int main(void)
{
	enum { NR_ITERATIONS = 100 };
	void *context;
	void *receiver;
	char buf[1];
	int64_t start_time;
	int i;
	int rc;

	context = zmq_ctx_new();
	assert(context != NULL);

	receiver = zmq_socket(context, ZMQ_PULL);
	assert(receiver != NULL);
	rc = zmq_bind(receiver, "tcp://*:5558");
	assert(rc == 0);

	// Wait for start of batch.
	rc = zmq_recv(receiver, buf, sizeof buf, 0);
	assert(rc == 1);

	start_time = s_clock();
	for (i = 0; i < NR_ITERATIONS; ++i) {
		rc = zmq_recv(receiver, buf, sizeof buf, 0);
		assert(rc == 0);
		putc(i % 10 == 0 ? ':' : '.', stderr);
	}
	fprintf(stderr, "Total elapsed time: %lld ms\n",
		s_clock() - start_time);

	rc = zmq_close(receiver);
	assert(rc == 0);
	rc = zmq_ctx_term(context);
	assert(rc == 0);
	return 0;
}
