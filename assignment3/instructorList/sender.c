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

// Sender thread declaration 
static pthread_t threadSender;

// Static declaration
static pthread_cond_t senderCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t senderMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t* send_shared_CS_mutex;
static char message[MSG_MAX_LEN];
static int LocalSocket;
static struct sockaddr_in* Remote;
static unsigned int addrSize;

// Extern declaration
extern List* sendMessageList;


// Sender thread function
void* senderThread(void* unused) 
{
    // Use to store message pointer pop out fron list
    char* messagePtr;

    while(1)
    {       
        // MessageList empty sender need to wait
        if (List_count(sendMessageList) == 0){
            Sender_wait();
        }

        // Critical section
        pthread_mutex_lock(send_shared_CS_mutex);
        {
            // Make first item the current item, because it's FIFO
            List_first(sendMessageList);
            messagePtr = (char*)(List_remove(sendMessageList));
            if (List_count(sendMessageList) != 0)
                // Now current is after the end, set it to previous one if not empty
                List_next(sendMessageList);
        }
        pthread_mutex_unlock(send_shared_CS_mutex);

        // There is avaliable buffer, signal input thread
        Input_signal();

        // Copy message to local message  
        for (int i = 0; i < MSG_MAX_LEN; i++){
            message[i] = *(messagePtr + i);
            if (*(messagePtr + i) == 0){
                break;
            }
        }

        free(messagePtr);
        messagePtr = NULL;

        // Send message to remote 
        int bytesSend = sendto(LocalSocket, message, strlen(message), 0, (struct sockaddr *) Remote, addrSize);
        //printf("In sender the send bytes is: %x\n", bytesSend);
        if (bytesSend < 0){
            printf("sendto failure\n");
            exit(1);
        }

        // if sent message is '!', end s-talk
        if (message[0] == 33 && (message[1]== 10 || message[1] == 0)){
            Input_cancel();
            Sender_cancel();
            Receiver_cancel();
            Printer_cancel();
        }
    }

    return NULL;
}

// Initialize the sender thread
void Sender_init(pthread_mutex_t* CS_mutex, int localsocket, Sockaddr* remote, unsigned int size)
{
	pthread_create(&threadSender, NULL, senderThread, NULL);
    send_shared_CS_mutex = CS_mutex;
    LocalSocket = localsocket;
    Remote = remote;
    addrSize = size;
}

// Block the sender thread untill signalled
void Sender_wait()
{
	pthread_mutex_lock(&senderMutex);
    {
        pthread_cond_wait(&senderCondVar, &senderMutex);
    }
    pthread_mutex_unlock(&senderMutex);
}

// Unblock the sender thread
void Sender_signal()
{
	pthread_mutex_lock(&senderMutex);
    {
        pthread_cond_signal(&senderCondVar);
    }
    pthread_mutex_unlock(&senderMutex);
}

// Main thread wait until this sender thread finish
void Sender_waitForShutdown()
{
	pthread_join(threadSender, NULL);
}

// Cancel the sender thread 
void Sender_cancel()
{
	pthread_cancel(threadSender);
}