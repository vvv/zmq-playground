/// Task worker.
///
/// Connects PULL socket to tcp://localhost:5557.
/// Collects workloads from ventilator via that socket.
/// Connects PUSH socket to tcp://localhost:5558.
/// Sends results to sender via that socket.

#include <zmq.h>
#include <assert.h>
#include <stdlib.h>  // atoi
#include <string.h>  // strlen
#include "helpers.h"

int main(void)
{
	void *context;
	void *receiver;
	void *sender;
	char *str;
	int rc;

	context = zmq_ctx_new();
	assert(context != NULL);

	receiver = zmq_socket(context, ZMQ_PULL);
	assert(receiver != NULL);
	rc = zmq_connect(receiver, "tcp://localhost:5557");
	assert(rc == 0);

	sender = zmq_socket(context, ZMQ_PUSH);
	assert(sender != NULL);
	rc = zmq_connect(sender, "tcp://localhost:5558");
	assert(rc == 0);

	while (1) {
		str = s_recv(receiver);
		assert(str != NULL);
		fprintf(stderr, "%s.", str);
		s_sleep(atoi(str));  // "do the work"
		free(str);
		rc = s_send(sender, "");
		assert(rc == 0);
	}
	rc = zmq_close(receiver);
	assert(rc == 0);
	rc = zmq_close(sender);
	assert(rc == 0);
	rc = zmq_ctx_term(context);
	assert(rc == 0);

	return 0;
}
