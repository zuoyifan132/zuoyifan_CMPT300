#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h> 

#include "list.h"
#include "input.h"
#include "receiver.h"
#include "printer.h"
#include "sender.h"


#define MSG_MAX_LEN 1024

// Input thread declaration 
static pthread_t threadReceiver;

// Static declaration
static pthread_cond_t receiverCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t receiverMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t* receive_shared_CS_mutex;
static char message[MSG_MAX_LEN];
static int LocalSocket;
static struct sockaddr_in* Remote;
static unsigned int* addrSize;

// Extern declaration
extern List* receiveMessageList;  

// Receiver thread function
void* receiverThread(void* unused) 
{
    while(1)
    {	
    	int bytesReceive = recvfrom(LocalSocket, message, MSG_MAX_LEN, 0, (struct sockaddr *) Remote, addrSize);
    	if (bytesReceive < 0){
    		printf("recvfrom failure\n");
            exit(1);
    	}

    	// Make it null terminated
		int terminateIdx = (bytesReceive < MSG_MAX_LEN) ? bytesReceive : MSG_MAX_LEN - 1;
		message[terminateIdx] = 0;

    	// Dynamically allocate a new buffer for each message 
        char* eachMessage = malloc(sizeof(char)*(strlen(message)+1));
        strcpy(eachMessage, message);


		// If List is full need to wait printer to signal receiver
		if (List_count(receiveMessageList) == LIST_MAX_NUM_NODES){
			printf("error: In receiver thread, List is full\n");
			Receiver_wait();
		}

		// Critical section
    	pthread_mutex_lock(receive_shared_CS_mutex);
    	{
    		List_append(receiveMessageList, eachMessage);
    	}
    	pthread_mutex_unlock(receive_shared_CS_mutex);

    	// There is message avaliable, signal printer
    	Printer_signal();
    }

    return NULL;
}

// Initialize the receiver thread
void Receiver_init(pthread_mutex_t* CS_mutex, int localsocket, Sockaddr* remote, unsigned int* size)
{
	pthread_create(&threadReceiver, NULL, receiverThread, NULL);
	receive_shared_CS_mutex = CS_mutex;
    LocalSocket = localsocket;
    Remote = remote;
    addrSize = size;
}

// Block the receiver thread untill signalled
void Receiver_wait()
{
	pthread_mutex_lock(&receiverMutex);
    {
        pthread_cond_wait(&receiverCondVar, &receiverMutex);
    }
    pthread_mutex_unlock(&receiverMutex);
}

// Unblock the receiver thread
void Receiver_signal()
{
	pthread_mutex_lock(&receiverMutex);
    {
        pthread_cond_signal(&receiverCondVar);
    }
    pthread_mutex_unlock(&receiverMutex);
}

// Main thread wait until this receiver thread finish
void Receiver_waitForShutdown()
{
	pthread_join(threadReceiver, NULL);
}

// Cancel the receiver thread 
void Receiver_cancel()
{
	pthread_cancel(threadReceiver);
}