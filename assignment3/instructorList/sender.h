#ifndef _SENDER_H_
#define _SENDER_H_

typedef struct sockaddr_in Sockaddr;

void Sender_init(pthread_mutex_t* CS_mutex, int localsocket, Sockaddr* remote, unsigned int size);
void Sender_wait();
void Sender_signal();
void Sender_cancel();
void Sender_waitForShutdown();

#endif