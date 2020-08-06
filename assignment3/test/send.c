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

#define MSG_MAX_LEN 100
//#define LOCAL_PORT 2000
//#define REMOTE_PORT 2001

int main(int argc, char** args)
{   
    // Check valid number of input
    if (argc != 3){
        printf("number of input incorrect\n");
        exit(-1);
    }

    int LOCAL_PORT = atoi(args[1]);
    int REMOTE_PORT = atoi(args[2]);

    //-------------------------------setup local socket--------------------------------------//

    // Address
    struct sockaddr_in Local;     
    memset(&Local, 0, sizeof(Local));          
    Local.sin_family = AF_INET;                   
    Local.sin_addr.s_addr = htonl(INADDR_ANY);    
    Local.sin_port = htons(LOCAL_PORT);

    // Create the socket for local socket and bind it
    int LocalSocket = socket(PF_INET, SOCK_DGRAM, 0);
    bind (LocalSocket, (struct sockaddr*) &Local, sizeof(Local));

    //---------------------------------------------------------------------------------------//

    //-------------------------------setup remote socket-------------------------------------//

    // Address
    struct sockaddr_in Remote;
    memset(&Remote, 0, sizeof(Remote));
    Remote.sin_family = AF_INET;                   
    Remote.sin_addr.s_addr = htonl(INADDR_ANY);    
    Remote.sin_port = htons(REMOTE_PORT);

    // Create the socket for remote socket and bind it
    int RemoteSocket = socket(PF_INET, SOCK_DGRAM, 0);
    bind (RemoteSocket, (struct sockaddr*) &Remote, sizeof(Remote));  

    //---------------------------------------------------------------------------------------//

    char Smessage[MSG_MAX_LEN];
    strcpy(Smessage, "fuck\n");

    unsigned int sin_len = sizeof(Local);
    int bytesSend = sendto(RemoteSocket, Smessage, strlen(Smessage), 0, (struct sockaddr *) &Local, sin_len);

    close(LocalSocket);
    close(RemoteSocket);

    return 0;
}