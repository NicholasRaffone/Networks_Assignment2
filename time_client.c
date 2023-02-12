#include<stdio.h> // header contaning functions such as printf
#include<stdlib.h> // header contaning functions such as malloc, free, atoi
#include<unistd.h> // for POSIX operating system API functions such as close
#include<string.h> //  header contaning functions such as memset
#include<sys/socket.h> // header contaning socket functions 
#include<arpa/inet.h> // header contaning byte-odering standardization functions of htons
#include<netinet/in.h> // header containing structure for internet addresses and macros such as INADDR_ANY 

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
 
    struct sockaddr_in srvaddr; // defines server address
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET; // IPv4 Family 
    srvaddr.sin_port = htons(9000); // convert 9000 port number to network-byte order
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char* request_message = "Time request";
    sendto(serverfd, request_message, strlen(request_message), 0, (struct sockaddr*) &srvaddr, sizeof(srvaddr)); // sent data to the server using the server address

    // Read the response from server into buffer (server time)
    unsigned int len = sizeof(srvaddr);
    char buffer[256];
    recvfrom(serverfd, buffer, 256, 0,(struct sockaddr *) &srvaddr, &len);
    
    printf("Server time: %s \n", buffer);
    close(serverfd);
}
