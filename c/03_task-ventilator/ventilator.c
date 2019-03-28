/// Task ventilator.
/// Binds PUSH socket to tcp://localhost:5557.
/// Sends batch of tasks to workers via that socket.

#include <zmq.h>
#include <assert.h>
#include <stdlib.h>  // srandom
#include <stdio.h>
#include <string.h>  // strlen
#include <time.h>
#include "helpers.h"

int main(void)
{
	enum { NR_ITERATIONS = 100 };
	void *context;
	void *sender;  // socket to send messages to
	void *sink;  // socket to send "start of batch" message to
	char str[10];
	int workload;
	int i;
	int total_msec = 0;
	int rc;

	context = zmq_ctx_new();
	assert(context != NULL);

	sender = zmq_socket(context, ZMQ_PUSH);
	assert(sender != NULL);
	rc = zmq_bind(sender, "tcp://*:5557");
	assert(rc == 0);

	sink = zmq_socket(context, ZMQ_PUSH);
	assert(sink != NULL);
	rc = zmq_connect(sink, "tcp://localhost:5558");
	assert(rc == 0);

	printf("Press Enter when the workers are ready... ");
	(void)getchar();

	fprintf(stderr, "Sending tasks to workers...\n");
	rc = s_send(sink, "0");  // "0" signals start of batch
	assert(rc == 1);

	srandom((unsigned)time(NULL));
	for (i = 0; i < NR_ITERATIONS; ++i) {
		workload = 1 + randof(100);
		total_msec += workload;
		rc = snprintf(str, sizeof str, "%d", workload);
		assert((size_t)rc < sizeof str);
		rc = s_send(sender, str);
		assert((size_t)rc == strlen(str));

	}
	rc = zmq_close(sink);
	assert(rc == 0);
	rc = zmq_close(sender);
	assert(rc == 0);
	rc = zmq_ctx_term(context);
	assert(rc == 0);

	fprintf(stderr, "Total expected cost: %d ms\n", total_msec);
	return 0;
}
