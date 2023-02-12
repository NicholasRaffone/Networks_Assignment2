#include<stdio.h> // header contaning functions such as printf
#include<stdlib.h> // header contaning functions such as malloc, free, atoi
#include<unistd.h> // for POSIX operating system API functions such as close
#include<string.h> //  header contaning functions such as memset
#include<sys/socket.h> // header contaning socket functions 
#include<arpa/inet.h> // header contaning byte-odering standardization functions of htons
#include<netinet/in.h> // header containing structure for internet addresses and macros such as INADDR_ANY 
#include <time.h> // header containing time setup

/*
 * STARTER CODE FROM: https://brightspace.nyu.edu/d2l/le/lessons/265424/topics/8356445
 */

int main()
{
    //socket()
    int serverfd = socket(AF_INET,SOCK_DGRAM,0); // creates a socket for communication via IPv4 and UDP
    if(serverfd<0)
    {
        perror("socket error");
        return -1;
    }

    if(setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&(int){1},sizeof(int))<0) // Allows to reuse address, without it you might run into "Address already in use errors"
    {
        perror("setsockopt error");
        exit(-1);
    }

    //bind()
    struct sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET; // IPv4 Family 
    srvaddr.sin_port = htons(9000); // convert port number 9000 to network-byte order
    
    
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(serverfd,(const struct sockaddr*)&srvaddr,sizeof(srvaddr))<0) // binds the socket to all the available network addresses which also include loopback (127.0.0.1) address
    {
        perror("bind error");
        return -1;
    }

    struct sockaddr_in cliaddr;
    bzero(&cliaddr,sizeof(cliaddr));
    unsigned int len = sizeof(cliaddr);

    printf("Time server started: \n");

    while(1)
    {
        char buffer[256];
        // Store received message into buffer, store client address to cliaddr
        recvfrom(serverfd, buffer, 256, 0,(struct sockaddr *) &cliaddr, &len);
        printf("%s from %s:%d\n",buffer, inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

        char current_time[256];
        //once we get the message, store current time as a string using ctime
        time_t timer = time(NULL);
        strcpy(current_time, ctime(&timer));

        // Send back stringified time data to client through cliaddr set from message
        sendto(serverfd,current_time,256,0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    }

    close(serverfd);
  
    return 0;
}

