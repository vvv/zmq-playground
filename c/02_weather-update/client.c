/// Weather update client.

#include <zmq.h>
#include <assert.h>
#include <string.h>  // strlen
#include <stdio.h>

int main(int argc, char **argv)
{
	enum { NR_ITERATIONS = 100 };
	void *context;
	void *subscriber;
	const char *filter;
	char buf[64];
	int zipcode, temperature, relhumidity;
	int sum = 0;
	int rc;
	int i;

	context = zmq_ctx_new();
	assert(context != NULL);

	subscriber = zmq_socket(context, ZMQ_SUB);
	assert(subscriber != NULL);

	rc = zmq_connect(subscriber, "tcp://localhost:5556");
	assert(rc == 0);

	// Subscribe to zipcode, default is NYC.
	filter = argc > 1 ? argv[1] : "10001";
	/*
	 * Note that when you use a SUB socket you *must* set a subscription
	 * using zmq_setsockopt() and ZMQ_SUBSCRIBE, as in this code.
	 * If you don't set any subscription, you won't get any messages.
	 *
	 * The subscriber can set many subscriptions, which are added together.
	 * That is, if an update matches ANY subscription, the subscriber
	 * receives it. The subscriber can also cancel specific subscriptions.
	 * A subscription is often, but not necessarily a printable string.
	 */
	rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
	assert(rc == 0);

	for (i = 0; i < NR_ITERATIONS; ++i) {
		rc = zmq_recv(subscriber, buf, sizeof buf, 0);
		assert(rc != -1 && (size_t)rc < sizeof buf);

		rc = sscanf(buf, "%d %d %d",
			    &zipcode, &temperature, &relhumidity);
		assert(rc == 3);
		sum += temperature;
	}
	rc = zmq_close(subscriber);
	assert(rc == 0);
	rc = zmq_ctx_term(context);
	assert(rc == 0);

	printf("Average temperature for zipcode '%s' was %d\n",
	       filter, sum / NR_ITERATIONS);

	return 0;
}
