#ifndef _HELPERS_H
#define _HELPERS_H

#include <stdint.h>  // int64_t

//  Provide random number from 0..(num-1)
long randof(long num);

/// Receive 0MQ string from socket and convert into C string.
/// Returns NULL if the context is being terminated.
/// Caller must free the returned string.
char *s_recv(void *socket);

/// Convert C string to 0MQ string and send to socket.
int s_send(void *socket, const char *str);

/// Sleep for a number of milliseconds.
void s_sleep(unsigned int msecs);

int64_t s_clock(void);

#endif  // _HELPERS_H
