#ifndef _RECEIVER_H_
#define _RECEIVER_H_

typedef struct sockaddr_in Sockaddr;

void Receiver_init(pthread_mutex_t* CS_mutex, int localsocket, Sockaddr* remote, unsigned int* size);
void Receiver_wait();
void Receiver_signal();
void Receiver_cancel();
void Receiver_waitForShutdown();

#endif