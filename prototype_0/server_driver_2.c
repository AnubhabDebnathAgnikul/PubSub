#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>


#define PORT 5678
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024


char server_msg[BUFFER_SIZE];
int N=0;


int server()
{
    /*Defining Variables*/
    int server_fd, new_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    int reuse = 1,on=1;
    int addrlen = sizeof(server_addr);

    /*create server socket*/
    if((server_fd = socket(AF_INET, SOCK_STREAM,0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /* Set socket to be nonblocking. All of the sockets for the incoming connections will also be nonblocking since */
    

    if ((ioctl(server_fd, FIONBIO, (char *)&on)) < 0)
    {
        perror("ioctl() failed");
        close(server_fd);
        exit(-1);
    }


    /*Bind the socket*/
    if((bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))<0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    /*Listen for incoming Connections*/
    
    if (listen(server_fd,MAX_CLIENTS)<0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("\nServer Listening on port %d ...",PORT);
    fflush(stdout);


    /*Poll fds*/
    struct pollfd fds[MAX_CLIENTS + 1];  // Additional slot for the server socket
    memset(fds, 0, sizeof(fds));

    fds[0].fd = server_fd;  // Server socket
    fds[0].events = POLLIN | POLLOUT;

    while(1)
    {
        int activity = poll(fds, MAX_CLIENTS + 1, -1);


        if(activity<0)
        {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        /*incoming connections*/
        if(fds[0].revents & POLLIN)
        {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) < 0) 
            {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }   

            // Send welcome message
            char welcome_msg[] = "Welcome to the server!";
            send(new_socket, welcome_msg, strlen(welcome_msg), 0);

            // Add the new client socket to the list
            for(int i=1 ; i<MAX_CLIENTS; i++)
            {
                if(fds[i].fd == 0)
                {
                    fds[i].fd = new_socket;
                    fds[i].events = POLLIN | POLLOUT;
                    printf("\nNew client connected, socket fd is %d\n", new_socket);
                    break;
                }
            }
        }

        /*Disconnections*/ 
        for (int i = 1; i <= MAX_CLIENTS; i++) 
        {
            if (fds[i].revents & POLLIN) 
            {
                int client_socket = fds[i].fd;
                char buffer[BUFFER_SIZE];
                int valread = read(client_socket, buffer, BUFFER_SIZE);

                if (valread <= 0) 
                {
                    // Client disconnected
                    printf("\nClient disconnected, socket fd is %d\n", client_socket);
                    close(client_socket);
                    fds[i].fd = 0;
                } 
                else 
                {
                    // Echo the message to all clients
                    for (int j = 1; j <= MAX_CLIENTS; j++) 
                    {
                        if (fds[j].fd != 0 && fds[j].fd != client_socket) 
                        {
                            send(fds[j].fd, buffer, valread, 0);
                        }
                    }
                }
            }
        }
        /*Broadcasting*/
        if(N!=0)
        {
            for (int i = 1; i < MAX_CLIENTS; i++) 
            {
                if (fds[i].fd != 0) 
                {
                    send(fds[i].fd, server_msg, strlen(server_msg), MSG_DONTWAIT);
                }
            }
            N=0;
        }
    }

}

/* Waiting for Broadcast Message */
int fun()
{
    while(1)
    {
        printf("\nenter message:  ");
        fgets(server_msg, BUFFER_SIZE, stdin);        
        N=1;
    }
}

/* Driver */ 
int main()
{
    pthread_t thread_id1, thread_id2;

    /*
        publisher_init()
        {
            if(no clash)
            pthread_create(&thread_id1,NULL, (void *)server,NULL);   
            pthread_create(&thread_id2,NULL, (void *)publish,NULL);
        }
    */

    pthread_create(&thread_id1,NULL, (void *)server,NULL);
    pthread_create(&thread_id2,NULL, (void *)fun,NULL);

    pthread_exit(NULL);
}