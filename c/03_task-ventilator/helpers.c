#include "helpers.h"

#include <zmq.h>
#include <stdlib.h>  // random
#include <string.h>  // strndup
#include <time.h>
#include <sys/time.h>
#include <assert.h>

long randof(long num)
{
	return num * random() / (RAND_MAX + 1L);
}

char *s_recv(void *socket)
{
	char buf[256];
	int size = zmq_recv(socket, buf, sizeof buf - 1, 0);

	if (size == -1)
		return NULL;
	buf[size] = '\0';
	return strndup(buf, sizeof buf - 1);
}

int s_send(void *socket, const char *str)
{
    return zmq_send(socket, str, strlen(str), 0);
}

void s_sleep(unsigned int msecs)
{
	const struct timespec t = {
		.tv_sec = msecs / 1000,
		.tv_nsec = (msecs % 1000) * 1000000
	};
	nanosleep(&t, NULL);
}

int64_t s_clock(void)
{
	struct timeval tv;
	int rc;

	rc = gettimeofday(&tv, NULL);
	assert(rc == 0);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
