/// Weather update server.

#include <zmq.h>
#include <assert.h>
#include <stdlib.h>  // srandom
#include <time.h>
#include <stdio.h>

static void get_weather(char *buf, size_t bufsize)
{
	int zipcode;
	int temperature;
	int relhumidity;
	int rc;

	assert(buf != NULL && bufsize > 0);

	zipcode = random() % 100000;
	temperature = random() % 215 - 80;
	relhumidity = random() % 50 + 10;

	rc = snprintf(buf, bufsize, "%05d %d %d",
		      zipcode, temperature, relhumidity);
	assert((size_t)rc < bufsize);
}

int main(void)
{
	void *context;
	void *publisher;
	int rc;
	char buf[32];

	context = zmq_ctx_new();
	assert(context != NULL);

	publisher = zmq_socket(context, ZMQ_PUB);
	assert(publisher != NULL);

	rc = zmq_bind(publisher, "tcp://*:5556");
	assert(rc == 0);

	srandom((unsigned)time(NULL));
	while (1) {
		get_weather(buf, sizeof buf);
		rc = zmq_send(publisher, buf, sizeof buf, 0);
		assert(rc == sizeof buf);
	}
	return 0;
}
