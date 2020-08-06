#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "input.h"
#include "receiver.h"
#include "printer.h"
#include "sender.h"

#define MSG_MAX_LEN 1024

// Input thread declaration 
static pthread_t threadInput;

// Static declaration
static pthread_cond_t inputCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t* send_shared_CS_mutex;
static char message[MSG_MAX_LEN];

// Extern declaration
extern List* sendMessageList;


// Input thread function 
void* inputThread(void* unused) 
{   
	while (1)
	{
        // Wait for user input
        if (fgets(message, MSG_MAX_LEN, stdin) == NULL){
            // Critical section
            Input_cancel();
            Sender_cancel();
            Receiver_cancel();
            Printer_cancel();
        }   

        // Change '\n' to null terminator
        int i = 0;
        while(i < 1024){
            if (message[i] == 10 || message[i] == 0){
                message[i] = 0;
                break;
            }
            i++;
        }
        i = 0;

        // Dynamically allocate a new buffer for each message 
        char* eachMessage = malloc(sizeof(char)*(strlen(message)+1));
        strcpy(eachMessage, message);
    
    	// List is full input thread need to wait sender to signal
    	if (List_count(sendMessageList) == LIST_MAX_NUM_NODES){
            printf("error: In input thread, List is full\n");
    		Input_wait();
        }

    	// Critical section
    	pthread_mutex_lock(send_shared_CS_mutex);
    	{
    		List_append(sendMessageList, eachMessage);
    	}
    	pthread_mutex_unlock(send_shared_CS_mutex);

    	// There is message avaliable, signal sender
    	Sender_signal();

        // if read a ! stop here
        if (message[0] == 33 && (message[1]== 10 || message[1] == 0))
            Input_cancel();
	}	

    return NULL;
}

// Initialize the input thread
void Input_init(pthread_mutex_t* CS_mutex)
{
	pthread_create(&threadInput, NULL, inputThread, NULL);
	send_shared_CS_mutex = CS_mutex;
}

// Block the input thread untill signalled
void Input_wait()
{
	pthread_mutex_lock(&inputMutex);
    {
        pthread_cond_wait(&inputCondVar, &inputMutex);
    }
    pthread_mutex_unlock(&inputMutex);
}

// Unblock the input thread
void Input_signal()
{
	pthread_mutex_lock(&inputMutex);
    {
        pthread_cond_signal(&inputCondVar);
    }
    pthread_mutex_unlock(&inputMutex);
}

// Main thread wait until this input thread finish
void Input_waitForShutdown()
{
	pthread_join(threadInput, NULL);
}

// Cancel the input thread 
void Input_cancel()
{
	pthread_cancel(threadInput);
}


