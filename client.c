
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

//Обертки для исключения ошибок
#include "errorprog.h"

struct Message {
    uint16_t header;
    uint64_t data;
};


int main() {

    const uint16_t headC = 0xA1B2;
    const uint16_t headS = 0xB2A1;

    int clientfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in client_address = {0};
    client_address.sin_family = AF_INET;  
    client_address.sin_port = htons(13453);
    Inet_pton(AF_INET, "127.0.0.1", &client_address.sin_addr);

    //socklen_t client_len = sizeof (client_address);

    Connect(clientfd, (struct sockaddr *) &client_address , sizeof (client_address)); 

    
    struct Message message;
    message.header = headC; 
    struct timeval start;
 
    gettimeofday(&start, NULL);
    uint64_t t =  (start.tv_sec * 1000000) + start.tv_usec;
    printf("time: %ld \n", t);
    
    message.data = t;
    ssize_t nwrite = 0;                  //Количество записанных байт
    ssize_t nread = 0; 

    //const time_t timer = time(NULL);
    nwrite = write(clientfd, &message.header, sizeof(message.header));
    nwrite+= write(clientfd, &message.data, sizeof(message.data));
    //send(clientfd, message.header, sizeof(message.header), 0);
   // send(clientfd, message.data, sizeof(message.data), 0);

    if(nwrite <= -1)
    {
        perror("write failed");
            exit(EXIT_FAILURE);
    }

    nread = read(clientfd, &message.header, sizeof(message.header));     
    nread += read(clientfd, &message.data, sizeof(message.data));
    
    if(nread <= -1)
    {
        perror("read failed");
            exit(EXIT_FAILURE);
    }
    if((uint16_t)message.header == (uint16_t)headS)
    {
        printf("dt = %ld \n", message.data);
    }else  printf("header false");

    close(clientfd);
    return 0;
}