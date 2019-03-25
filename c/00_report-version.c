#if 0
set -e
gcc -Wall -W -Werror -l zmq -o ${0%.c} $0 $*
exec ${0%.c} "$@"
#else

#include <zmq.h>
#include <stdio.h>

int main(void)
{
	int major, minor, patch;

	zmq_version(&major, &minor, &patch);
	printf("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
	return 0;
}

#endif
