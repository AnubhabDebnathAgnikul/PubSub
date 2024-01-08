#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

// #include "common.h"

#define PORT 1234
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 10
#define TIMEOUT_USEC 100000

int main()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = TIMEOUT_USEC;

    // Wait for data to be received with timeout
    int select_status = select(sock + 1, &readfds, NULL, NULL, &timeout);
    if (select_status == -1) 
    {
        perror("Error in select");
        exit(EXIT_FAILURE);
    } 
    else if (select_status == 0) 
    {
        printf("Timeout error: No data received within the specified timeout\n");
        exit(EXIT_FAILURE);
    }

    while(buffer!="stop")
    {
        // Data is available, receive the message from the server
        valread = read(sock, buffer, BUFFER_SIZE);
        if(valread=='\0')
        {
            break;
        }
        printf("Received message from server: %s\n", buffer);
        memset(buffer,0,BUFFER_SIZE);
    }
    
    close(sock);

    // return 0;
}