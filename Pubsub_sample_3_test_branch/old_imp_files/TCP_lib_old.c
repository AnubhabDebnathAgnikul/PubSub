// Common - Process (Pub/Sub)
 
// User Function definition 
#include "TCP_lib_old.h"
#include "SHM_lib.h"
#include "string_lib.h"
#include "DiscMsg_check.h"
#include "pthread.h"
#include "../config.h"

#define MAX_BUFFER_SIZE 10000  

// Send message to all the clients 
STATUS_CHECK TCP_server(char* port_msg, char* g_tx_buffer) 
{
    
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    // struct pollfd fds_poll[1];
    int client_sockets[5];
    socklen_t client_addr_len = sizeof(client_addr);
    int port_int;
    int reuse = 1;

    char internal_buff[2500];


    // Creating socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket creation failed");
        return FAIL;
    }
    
    // Type conversion string -> int
    port_int = atoi(port_msg);

    // Config settings
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_int);

    // reuse port and address 
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        exit(EXIT_FAILURE);
    }


    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("Binding failed");
        return FAIL;
    }

    // listen
    if (listen(server_socket, 5) == -1) 
    {
        perror("Listen failed");
        return FAIL;
    }

    
    printf("[+] TCP : TCP Server created and listening on Port ID: %d......\n", port_int);

    // accept 
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) 
    {
        perror("Acceptance failed");
        return FAIL;

    }

    int i = 0;

    for (i ; i < 20000  ; )
    { 
        i++;

        if (send(client_socket, g_tx_buffer, strlen(g_tx_buffer), 0) < 0)
        {
            perror("send FAIL");
            return FAIL;
        }
                        
        printf("[+] TCP : Message sent to client successfully : Message = %s pkt count : %d\n",g_tx_buffer,i);
                    
    }
         

    // Closing both client and server sockets
    close(client_socket);
    close(server_socket);
    
    return PASS;
}



// #define MAX_SERVERS_CONN 2

// SHM parameters 
size_t SHM_SIZE = 1000;
char* mapping_ptr_1;
char g_shm_buff[200]; 

// Only to receive and send to other servers 
STATUS_CHECK TCP_server_poll_update(char* port_msg, char* g_recv_buff,char* SHM_name)
{

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    struct pollfd fds_poll[1];
    socklen_t client_addr_len = sizeof(client_addr);
    int port_int;
    int reuse = 1; // Variable for reuasability of port 

    char internal_buff[2500]; // Buffer in which the message will received from user TCP_Client() will be stored
    char TCP_send_buff[2000];
    char SHM_check_buff[50];
    char TCP_tmp_buff[50];

    // Creating socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket creation failed");
        return FAIL;
    }
    
    // Type conversion string -> int
    port_int = atoi(port_msg);

    // Config settings
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_int);

    // reuse port and address 
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        exit(EXIT_FAILURE);
    }


    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("Binding failed");
        return FAIL;
    }

    // listen
    if (listen(server_socket, 5) == -1) 
    {
        perror("Listen failed");
        return FAIL;
    }

    printf("[+] TCP : TCP Server created and listening on Port ID: %d......\n", port_int);

    memset(fds_poll, 0, sizeof(fds_poll));
    
    // Poll struct params 
    fds_poll[0].fd = server_socket;
    fds_poll[0].events = POLLIN;

    // Super - loop start 
    while (1)
    {  

        int ready = poll(fds_poll, 1, -1);

        
        if (ready == -1) 
        {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        }
    
        if (fds_poll[0].revents & POLLIN) 
        {
            
            // New connection is accepted  
            if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) 
            {
                perror("Error accepting connection");
                return FAIL;
            }
        
            // Reading from client socket at particular PORT 
            if ((read(client_socket, internal_buff, sizeof(internal_buff))) < 0)
            {
                perror("Server Disconnected");
                return FAIL;
            }

            memcpy(g_recv_buff, internal_buff, strlen(internal_buff) + 1);

            memcpy(TCP_tmp_buff, internal_buff, strlen(internal_buff) + 1);


            // Tokenizing and getting the status value 

            int disc_msg_status = discmesgcheck(SHM_name, internal_buff, SHM_check_buff);

            printf("status message %d \n", disc_msg_status);
            
            // Sending to the same client socket

            if (disc_msg_status == ALL_PASS || disc_msg_status == NOT_OK)
            {
                if ((send(client_socket, "4", sizeof("4"), 0)) == -1)
                {
                    printf("TCP send failure");
                    return FAIL;
                }

                SHM_init_write(SHM_name, SHM_SIZE, mapping_ptr_1, g_recv_buff);

                for (int i = 0; i < MAX_SYNC_SERV ; i++)
                {
                    
                    TCP_Client(SYNC_CLIENT_PORTS[i], SYNC_CLIENT_IPS[i], TCP_tmp_buff, NULL);
                
                }
            }

            else if (disc_msg_status == STAT_FAIL || disc_msg_status == PORT_FAIL)
            {
                if (disc_msg_status == STAT_FAIL)
                {
                    if ((send(client_socket, "2", sizeof("2"), 0)) < 0)
                    {
                        printf("TCP send failure");
                        return FAIL;
                    }
                }
                
                else if (disc_msg_status == PORT_FAIL)
                {
                    if ((send(client_socket, "3", sizeof("3"), 0)) < 0)
                    {
                        printf("TCP send failure");
                        return FAIL;
                    }
                }   
            }


            // Buffer Clearing 
            
            memset(internal_buff, 0, sizeof(internal_buff));

            memset(g_recv_buff, 0, sizeof(g_recv_buff));

            memset(TCP_tmp_buff, 0, sizeof(TCP_tmp_buff));

            // SHM_del(SHM_name);
        }

    }

    // Closing both client and server sockets
    // Note : Will never ever reach here , continuosly in listening mode 
    close(client_socket);
    close(server_socket);

    return PASS;
}


STATUS_CHECK TCP_server_poll(char* port_msg, char* g_recv_buff,char* SHM_name,int stat_flag)
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    struct pollfd fds_poll[1];
    socklen_t client_addr_len = sizeof(client_addr);
    int port_int;
    int reuse = 1;

    char internal_buff[25000];


    // Creating socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket creation failed");
        return FAIL;
    }
    
    // Type conversion string -> int
    port_int = atoi(port_msg);

    // Config settings
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_int);

    // reuse port and address 
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        exit(EXIT_FAILURE);
    }


    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("Binding failed");
        return FAIL;

    }

    // listen
    if (listen(server_socket, 10) == -1) 
    {
        perror("Listen failed");
        return FAIL;
    }

    printf("[+] TCP : TCP Server created and listening on Port ID: %d......\n", port_int);

    memset(fds_poll, 0, sizeof(fds_poll));
    
    // Poll struct params 
    fds_poll[0].fd = server_socket;
    fds_poll[0].events = POLLIN;

    // Super - loop start 
    while (1)
    {  

        // status_flag = 0;

        int ready_act = poll(fds_poll, 1, -1);

        
        if (ready_act == -1) 
        {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        }
    
        if (fds_poll[0].revents & POLLIN) 
        {
        

            // New connection accept 
            if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) 
            {
                perror("Error accepting connection");
                exit(EXIT_FAILURE);
            }
        
            // Reading from client socket at particular PORT 
            if ((read(client_socket, internal_buff, sizeof(internal_buff))) < 0)
            {
                printf("Server Disconnected");
                return FAIL;
            }

            memcpy(g_recv_buff, internal_buff, strlen(internal_buff) + 1);

            printf("message in the buffer : %s \n",g_recv_buff);

            SHM_init_write(SHM_name, SHM_SIZE, mapping_ptr_1, g_recv_buff);

            memset(internal_buff,0,sizeof(internal_buff));

            memset(g_recv_buff,0,sizeof(g_recv_buff));
    
        }

    }

    // Closing both client and server sockets
    // Note : Will never ever reach here , continuosly in listening mode 
    close(client_socket);
    close(server_socket);

    return PASS;
}


STATUS_CHECK TCP_Client(char* port_conn,char* ip_addr_conn, char* tx_buffer, char* rx_buffer)
{
    int socket_fd = 0;
    struct sockaddr_in serv_addr;
    int port_int;
    int reuse = 1;
    // char buffer[1024] = {0};

    // Create socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("Socket creation error \n");
        return FAIL;
    }

    // Type conversion string -> int
    port_int = atoi(port_conn);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_int);

    // Port and IP addr reusability 
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        exit(EXIT_FAILURE);
    }

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_addr_conn, &serv_addr.sin_addr) <= 0) 
    {
        printf("Invalid address Address not supported \n");
        return FAIL;
    }

    // Connect to server
    // while(1)
    // {
    
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("Connection Failed \n");
        return FAIL;
    }

    // Send message to server
    if (send(socket_fd, tx_buffer, strlen(tx_buffer), 0) < 0)
    {
        perror("Send Failure \n");
        return FAIL;
    }

    if (rx_buffer != NULL) 
    {
        // Receive Reply from server 
        if (read(socket_fd, rx_buffer, sizeof(rx_buffer)) < 0)
        {
            perror("Read Failure \n");
            return FAIL;
        }

    }
    
    printf("[+] TCP : Message sent to server successfully = %s\n",tx_buffer);

    printf("[+] TCP : Reply Message from the Sync Server = %s\n",rx_buffer);

    printf("[+] TCP : TCP client closed.\n");
    
    close(socket_fd);
    
    return PASS;
}


STATUS_CHECK TCP_Client_Cont(char* port_conn,char* ip_addr_conn, char* g_rx_buffer)
{
    int socket_fd = 0;
    struct sockaddr_in serv_addr;
    int port_int;
    int reuse = 1;

    char internal_buff[2500];

    // Create socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\n Socket creation error \n");
        return FAIL;
    }

    // Type conversion string -> int
    port_int = atoi(port_conn);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_int);

    // Port and IP addr reusability 
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        exit(EXIT_FAILURE);
    }

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_addr_conn, &serv_addr.sin_addr) <= 0) 
    {
        printf("\nInvalid address Address not supported \n");
        return FAIL;
    }

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("Connection Failed \n");
        return FAIL;
    }
    while (1)
    {
        
        // Reading from server socket at particular PORT 
        if ((read(socket_fd, internal_buff, sizeof(internal_buff))) <= 0)
        {
            printf("[+] TCP : Read Failure sock closed\n");
            // return FAIL;
            break;
        }
        
        // Copying the contents of internal_buffer to recv_buffer 
        memcpy(g_rx_buffer, internal_buff, sizeof(internal_buff));
        fflush(stdout);

        // usleep(1000);
        printf("message in recv Buff = %s\n", g_rx_buffer);
        
        memset(internal_buff, 0, sizeof(internal_buff));
        // memset(g_recv_buff, 0, sizeof(g_recv_buff)); 
       
    }
    
    printf("[+] TCP : TCP client closed.\n");
    close(socket_fd);
    return PASS;
}


STATUS_CHECK TCP_Client_poll(char* port_conn,char* ip_addr_conn, char* tx_buffer)
{
    
    int socket_fd = 0;
    struct sockaddr_in serv_addr;
    int port_int;
    struct pollfd fds_poll[1];
    // char buffer[1024] = {0};

    // Create socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\n Socket creation error \n");
        return FAIL;
    }

    // Type conversion string -> int
    port_int = atoi(port_conn);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_int);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_addr_conn, &serv_addr.sin_addr) <= 0) 
    {
        printf("\nInvalid address Address not supported \n");
        return FAIL;
    }


    memset(fds_poll, 0, sizeof(fds_poll));
    
    // Poll struct params 
    fds_poll[0].fd = socket_fd;
    fds_poll[0].events = POLLOUT;

    while (1)
    {  

        int ready = poll(fds_poll, 1, 1000);

        
        if (ready == -1) 
        {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        }


    if (fds_poll[0].revents & POLLOUT) 
    {

        if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        {
            printf("Connection Failed \n");
            return FAIL;
        }

        // Send message to server
        if (send(socket_fd, tx_buffer, strlen(tx_buffer), 0) < 0)
        {
            return FAIL;
        }

        // usleep(10000);
    
        printf("[+] TCP : Message sent to server : Message = %s\n",tx_buffer);
    
        // printf("[+] TCP : TCP client closed.\n");
    
        // memset(tx_buffer, 0, strlen(tx_buffer) + 1);
    }
    }

    // Will never reach here 

    close(socket_fd);
    
    return PASS;

}
