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
static pthread_t threadPrinter;

// Static declaration
static pthread_cond_t printerCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t printerMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t* receive_shared_CS_mutex;
static char message[MSG_MAX_LEN];

// Extern declaration
extern List* receiveMessageList;  

// Printer thread function
void* printerThread(void* unused) 
{
    // Use to store message pointer pop out fron list
    char* messagePtr;

    while(1)
    {
        // If messageList empty printer need to wait receiver signal
        if (List_count(receiveMessageList) == 0)
            Printer_wait();

        // Critical section
        pthread_mutex_lock(receive_shared_CS_mutex);
        {
            // Make first item the current item, because it's FIFO
            List_first(receiveMessageList);
            messagePtr = (char*)(List_remove(receiveMessageList));
            if (List_count(receiveMessageList) != 0)
                // Now current is after the end, set it to previous one if not empty
                List_next(receiveMessageList);
        }
        pthread_mutex_unlock(receive_shared_CS_mutex);

        // There is avaliable buffer, signal receiver thread
        Receiver_signal();

        // Copy message to local message  
        for (int i = 0; i < MSG_MAX_LEN; i++){
            message[i] = *(messagePtr + i);
            if (*(messagePtr + i) == 0)
                break;
        }

        free(messagePtr);

        // Print message 
        write(STDOUT_FILENO, message, strlen(message));
        write(STDOUT_FILENO, "\n", strlen("\n"));
        fflush(stdout);

        // if receive message is '!', end s-talk
        if (message[0] == 33 && (message[1]== 10 || message[1] == 0)){
            Input_cancel();
            Sender_cancel();
            Receiver_cancel();
            Printer_cancel();
        }
    }

    return NULL;
}

// Initialize the printer thread
void Printer_init(pthread_mutex_t* CS_mutex)
{
	pthread_create(&threadPrinter, NULL, printerThread, NULL);
    receive_shared_CS_mutex = CS_mutex;
}

// Block the printer thread untill signalled
void Printer_wait()
{
	pthread_mutex_lock(&printerMutex);
    {
        pthread_cond_wait(&printerCondVar, &printerMutex);
    }
    pthread_mutex_unlock(&printerMutex);
}

// Unblock the printer thread
void Printer_signal()
{
	pthread_mutex_lock(&printerMutex);
    {
        pthread_cond_signal(&printerCondVar);
    }
    pthread_mutex_unlock(&printerMutex);
}

// Main thread wait until this printer thread finish
void Printer_waitForShutdown()
{
	pthread_join(threadPrinter, NULL);
}

// Cancel the printer thread 
void Printer_cancel()
{
	pthread_cancel(threadPrinter);
}