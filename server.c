#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>





//Обертки для исключения ошибок
#include "errorprog.h"

struct Message {
    uint16_t header;
    uint64_t data;
};



int main()
{
    const uint16_t headC = 0xA1B2;
    const uint16_t headS = 0xB2A1;
    

    int serverfd = Socket(AF_INET, SOCK_STREAM, 0);

    //задание адреса по протоколу IPv4
    //struct sockaddr_un adr = {0}; 
    struct sockaddr_in server_address = {0}; 

    //struct sockaddr_in client_address = {0}; 

    server_address.sin_family = AF_INET;                   //задание семейства адресов и порт, на котором сервер будет слушать (в своем порядке байт?)
    //server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(13453);
    


    socklen_t server_len = sizeof (server_address);
    Bind(serverfd, (struct sockaddr *) &server_address , server_len);           //привязка сокета к адресу (сокет для привязки, адрес, размер адреса)

    Listen(serverfd, 5);                         //Очередь до 5 клиентов?

    
    printf("server waiting\t");
    server_len = sizeof (server_address);

    //socklen_t client_len = sizeof (client_address);

    //Через данный дискриптор осуществляется общение с клиентом 
    //int clientfd = Accept(serverfd, (struct sockaddr *) &client_address, &client_len); //Создание нового соединения для клиента
    int clientfd = Accept(serverfd, (struct sockaddr *) &server_address, &server_len); //Создание нового соединения для клиента


    struct Message messageC;
    ssize_t nread;                  //Количество считанных байт
    //int siz = sizeof(uint16_t) + sizeof(uint64_t);
    //char buf[siz];
    //memset(buf, 0, siz);

    nread = read(clientfd, &messageC.header, sizeof(messageC.header));     
    nread += read(clientfd, &messageC.data, sizeof(messageC.data)); //Количество записанных байт
    
    if(nread <= -1)
    {
        perror("read failed");
            exit(EXIT_FAILURE);
    }

    if((uint16_t)messageC.header == (uint16_t)headC)
    {
        //Передача ответа клиенту

        //Подсчет разницы во времени 
        struct timeval end;
 
        gettimeofday(&end, NULL);
        uint64_t t =  (end.tv_sec * 1000000) + end.tv_usec;
        printf("time: %ld \n", t);
    
        messageC.data = t - messageC.data;
       // printf("dtime: %lu \n", messageC.data);
    
        ssize_t nwrite;                  //Количество записанных байт

        messageC.header = headS;

        nwrite = write(clientfd, &messageC.header, sizeof(messageC.header));
        nwrite+= write(clientfd, &messageC.data, sizeof(messageC.data));

    } else printf("header false");

    close(clientfd);
    close(serverfd);


    return 0;
    



}
