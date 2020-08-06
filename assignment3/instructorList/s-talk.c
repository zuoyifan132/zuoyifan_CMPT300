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

static pthread_mutex_t send_shared_CS_mutex = PTHREAD_MUTEX_INITIALIZER;    // Shared critical section problem mutex between other files
static pthread_mutex_t receive_shared_CS_mutex = PTHREAD_MUTEX_INITIALIZER; // Shared critical section problem mutex between other files
List* sendMessageList;                                                      // Shared messgaeList between other files
List* receiveMessageList;                                                   // Shared messgaeList between other files
static int LocalSocket;
static struct sockaddr_in Remote;
static unsigned int size;



int main(int argc, char** args)
{   
    // Check valid number of input
    if (argc != 4){
        printf("number of input incorrect\n");
        exit(-1);
    }

    // Store args information
    const int local_port = atoi(args[1]);
    const char* remote_name = args[2];
    const int remote_port = atoi(args[3]);


    //---------------------------- get remote machine IP address ----------------------------//

    // Declare varibles used for getaddrinfo
    struct addrinfo hints;
    struct addrinfo *res;
    struct in_addr addr;
    int error_code;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // Check getaddrinfo success or not
    if((error_code = getaddrinfo(remote_name, NULL, &hints, &res)) != 0){
        printf("error %d : %s\n", error_code, gai_strerror(error_code));
        return 1;
    }

    // Remote machine IP address
    addr.s_addr = ((struct sockaddr_in*)(res->ai_addr))->sin_addr.s_addr;

    //---------------------------------------------------------------------------------------//

    //-------------------------------setup local socket--------------------------------------//

    // Address
    struct sockaddr_in Local;     
    memset(&Local, 0, sizeof(Local));          
    Local.sin_family = AF_INET;                   
    Local.sin_addr.s_addr = htonl(INADDR_ANY);    
    Local.sin_port = htons(local_port);

    // Create the socket for local socket and bind it
    LocalSocket = socket(PF_INET, SOCK_DGRAM, 0);
    bind (LocalSocket, (struct sockaddr*) &Local, sizeof(Local));

    //---------------------------------------------------------------------------------------//

    //-----------------------------setup remote sockaddr_in----------------------------------//

    // Address
    memset(&Remote, 0, sizeof(Remote));
    Remote.sin_family = AF_INET;                   
    Remote.sin_addr.s_addr = addr.s_addr;    
    Remote.sin_port = htons(remote_port);

    size = sizeof(Remote);

    //---------------------------------------------------------------------------------------//      

    // The message list that will be manipulated
    sendMessageList = List_create();
    receiveMessageList = List_create();

    // Initialize all threads
    Receiver_init(&receive_shared_CS_mutex, LocalSocket, &Remote, &size);
    Sender_init(&send_shared_CS_mutex, LocalSocket, &Remote, size);
    Printer_init(&receive_shared_CS_mutex);
    Input_init(&send_shared_CS_mutex);

    // Main thread waits all four threads finish
    Receiver_waitForShutdown();
    Input_waitForShutdown();
    Sender_waitForShutdown();
    Printer_waitForShutdown();

    // Clean up
    close(LocalSocket);
    freeaddrinfo(res);

    return 0;
}