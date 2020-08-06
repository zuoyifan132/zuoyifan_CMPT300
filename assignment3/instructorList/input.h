#ifndef _INPUT_H_
#define _INPUT_H_

void Input_init(pthread_mutex_t* CS_mutex);
void Input_wait();
void Input_signal();
void Input_cancel();
void Input_waitForShutdown();

#endif