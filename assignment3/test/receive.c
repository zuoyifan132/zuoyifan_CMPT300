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
    Local.sin_addr.s_addr = htonl(INADDR_ANY);    //should use addr.s?
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

    char Rmessage[MSG_MAX_LEN];

    printf("before recvfrom()\n");
    unsigned int sin_len = sizeof(Remote);
    int bytesReceive = recvfrom(LocalSocket, Rmessage, MSG_MAX_LEN, 0, (struct sockaddr *) &Remote, &sin_len);
    printf("after recvfrom()\n");

    int terminateIdx = (bytesReceive < MSG_MAX_LEN) ? bytesReceive : MSG_MAX_LEN - 1;
        Rmessage[terminateIdx] = 0;

    printf("The received message is: %s\n", Rmessage);

    close(LocalSocket);
    close(RemoteSocket);

    return 0;
}