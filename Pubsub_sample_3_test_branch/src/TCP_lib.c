// TCP_lib.c - New Implementation of TCP lib 

// only contains 2 functions (Base functions) 
// 1. TCP_server()
// 2. TCP_client()

// Custom TCP_send_recv() functions 

#include "TCP_lib.h"
#include "SHM_lib.h"
#include "string_lib.h"
#include "DiscMsg_check.h"

// config header 
#include "../config.h"

// Server function with reusability PORT & IP_ADDR 
TCP_CHECK TCP_server(TCP_param_t *TCP_param, char* port_listen, int MAX_CLIENTS)
{
    TCP_param->reuse = 1; // Port and ip_addr reuse 
    
    TCP_param->tcp_flag = 0; // for server
    int port_listen_int = atoi(port_listen);
    socklen_t client_addr_len = sizeof(TCP_param->client_addr); 

    // Creating socket
    if ((TCP_param->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket creation failed");
        return TCP_FAIL;
    }

    // Config settings
    TCP_param->server_addr.sin_family = AF_INET;
    TCP_param->server_addr.sin_addr.s_addr = INADDR_ANY;
    TCP_param->server_addr.sin_port = htons(port_listen_int);

    // reuse port and address 
    if (setsockopt(TCP_param->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (&TCP_param->reuse), sizeof(TCP_param->reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        return TCP_FAIL;
    }

    // Bind
    if (bind(TCP_param->server_fd, (struct sockaddr *) (&TCP_param->server_addr), sizeof(TCP_param->server_addr)) == -1) 
    {
        perror("Binding failed");
        return TCP_FAIL;
    }

    printf("[+] TCP : TCP Server created and listening on Port ID: %d......\n", port_listen_int);

    // listen
    if (listen(TCP_param->server_fd, MAX_CLIENTS) == -1) 
    {
        perror("Listen failed");
        return TCP_FAIL;
    }


    // printf("server_fd : %d \n", TCP_param->server_fd);
    // printf("client_fd : %d\n", TCP_param->client_fd);
    // printf("New_sock_fd : %d \n", TCP_param->new_sock_fd);
    // printf("reuse : %d \n",TCP_param->reuse);

    return TCP_PASS;

}


TCP_CHECK TCP_client(TCP_param_t *TCP_param_1, char* port_conn, char* ip_conn)
{

    TCP_param_1->tcp_flag = 1; // '1' for usage client
    TCP_param_1->new_sock_fd = 0; // '1' for usage client
    TCP_param_1->reuse = 1;
    int port_conn_int = atoi(port_conn); // Type conversion string -> int

    // Create socket
    if ((TCP_param_1->new_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("\n Socket creation error \n");
        return TCP_FAIL;
    }

    TCP_param_1->server_addr.sin_family = AF_INET;
    TCP_param_1->server_addr.sin_port = htons(port_conn_int);

    // reuse port and address 
    if (setsockopt(TCP_param_1->new_sock_fd , SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (&TCP_param_1->reuse), sizeof(TCP_param_1->reuse)) < 0) 
    {
        perror("Setting SO_REUSEPORT failed");
        return TCP_FAIL;
    }

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_conn, &TCP_param_1->server_addr.sin_addr) <= 0) 
    {
        perror("Invalid address Address not supported \n");
        return TCP_FAIL;
    }    

    if (connect(TCP_param_1->new_sock_fd , (struct sockaddr *) (&TCP_param_1->server_addr), sizeof(TCP_param_1->server_addr)) < 0) 
    {
        perror("Connection Failed");
        return TCP_FAIL;
    }

    printf("[+] TCP : Connection success \n");

    // printf("server_fd : %d \n", TCP_param_1->server_fd);
    // printf("client_fd : %d\n", TCP_param_1->client_fd);
    // printf("New_sock_fd : %d \n", TCP_param_1->new_sock_fd);
    // printf("reuse : %d \n",TCP_param_1->reuse);

    return TCP_PASS;

}

/****************************************************************************************************************************************************************/
// Custom send and receive functions for TCP_client() and TCP_server()
// 1. TCP_send_recv() 
// 2. TCP_send_recv_cont() 
// 3. TCP_send_recv_poll()
// 4. TCP_send_recv_update()
/****************************************************************************************************************************************************************/

// for send and receive on a TCP socket 
TCP_CHECK TCP_send_recv(TCP_param_t *TCP_param_2, void *tx_buffer, void* rx_buffer)
{   
    socklen_t client_addr_len = sizeof(TCP_param_2->client_addr); 

    // For TCP server usage
    if( (TCP_param_2->tcp_flag == 0) )
    {   
        // accept 
        if ((TCP_param_2->client_fd = accept(TCP_param_2->server_fd, (struct sockaddr *) (&TCP_param_2->client_addr), &client_addr_len)) == -1) 
        {
            perror("Acceptance failed");
            return TCP_FAIL;
        }

        printf("[+] TCP : Accepted connection client_fd : %d \n", TCP_param_2->client_fd);
            
        if (tx_buffer != NULL)
        {
            if (send(TCP_param_2->client_fd, tx_buffer, strlen(tx_buffer),0) < 0)
            {
                perror("SEND_FAILURE \n");
                return TCP_FAIL;
            }
        }

        if (rx_buffer != NULL)
        {
            if (read(TCP_param_2->client_fd, rx_buffer, sizeof(rx_buffer)) < 0)
            {
                perror("RECV_FAILURE \n");
                return TCP_FAIL;
            }
        }
            
            // Socket closing
        printf("[+] TCP : server socket closed \n");

        close(TCP_param_2->server_fd);
        close(TCP_param_2->client_fd);
    
    }
    
    
    // For TCP_client() usage
    else if( (TCP_param_2->tcp_flag != 0) )
    {
        if (tx_buffer != NULL)
        {
            if (send(TCP_param_2->new_sock_fd, tx_buffer, strlen(tx_buffer),0) < 0)
            {
                perror("SEND_FAILURE \n");
                return TCP_FAIL;
            }
        }

        if (rx_buffer != NULL)
        {
            if (read(TCP_param_2->new_sock_fd, rx_buffer, sizeof(rx_buffer)) < 0)
            {
                perror("RECV_FAILURE \n");
                return TCP_FAIL;
            }
        }

        // Socket closing 
        printf("[+] TCP : client socket closed \n");

        close(TCP_param_2->new_sock_fd); 
    } 

    return TCP_PASS;
}

// for send and receive on a TCP socket 
TCP_CHECK TCP_send_recv_cont(TCP_param_t *TCP_param_2, void *tx_buffer, void* rx_buffer)
{   
    
    // For TCP server usage
    if( (TCP_param_2->tcp_flag == 0) )
    {
        socklen_t client_addr_len = sizeof(TCP_param_2->client_addr); 

        // accept 
        if ((TCP_param_2->client_fd = accept(TCP_param_2->server_fd, (struct sockaddr *) (&TCP_param_2->client_addr), &client_addr_len)) == -1) 
        {
            perror("Acceptance failed");
            return TCP_FAIL;
        }

        printf("[+] TCP : Accepted connection client_fd : %d \n", TCP_param_2->client_fd);

        int i = 0;    
            
        // while(1)
        for ( ; i < 20000 ; )
        {    
            i = i + 1;

            if (tx_buffer != NULL)
            {
                if (send(TCP_param_2->client_fd, tx_buffer, strlen(tx_buffer),0) < 0)
                {
                    perror("SEND_FAILURE \n");
                    return TCP_FAIL;
                }

                printf("message send to client %s, iteration : %d \n", (char*) tx_buffer, i);
            }

            if (rx_buffer != NULL)
            {
                if (read(TCP_param_2->client_fd, rx_buffer, sizeof(rx_buffer)) < 0)
                {
                    perror("RECV_FAILURE \n");
                    return TCP_FAIL;
                    // break;
                }
            }
        }
            
        // Socket closing
        printf("[+] TCP : server socket closed \n");

        close(TCP_param_2->server_fd);
        close(TCP_param_2->client_fd);

        return TCP_PASS;
    
    }
    
    
    // For TCP_client() usage
    else if( (TCP_param_2->tcp_flag != 0) )
    {
        char internal_buffer[200];
        
        while(1)
        {     
            if (tx_buffer != NULL)
            {
                if (send(TCP_param_2->new_sock_fd, tx_buffer, strlen(tx_buffer),0) < 0)
                {
                    perror("SEND_FAILURE \n");
                    return TCP_FAIL;
                }
            }

            if (rx_buffer != NULL)
            {
                if (read(TCP_param_2->new_sock_fd, internal_buffer, sizeof(internal_buffer)) <= 0)
                {
                    perror("RECV_FAILURE \n");
                    // return TCP_FAIL;
                    break;
                }

                memcpy(rx_buffer, internal_buffer, sizeof(internal_buffer));

                fflush(stdout);

                printf("message in recv Buff = %s \n", (char*) rx_buffer);

                memset(internal_buffer, 0, sizeof(internal_buffer));

            }
        }

        // Socket closing 
        printf("[+] TCP : client socket closed \n");

        close(TCP_param_2->new_sock_fd); 

        return TCP_PASS;
    } 

    
}


// for send and receive on a TCP socket in polling mode.
// Will be writing in SHM 
TCP_CHECK TCP_send_recv_poll(TCP_param_t *TCP_param_2, void *tx_buffer, void* rx_buffer, char* SHM_name)
{   
    struct pollfd fds_poll[1];
    memset(fds_poll, 0, sizeof(fds_poll));

    size_t SHM_SIZE = 1000;
    char* mapping_ptr_1; 
    
    // For TCP_server() usage
    if( (TCP_param_2->tcp_flag == 0) )
    {   
        socklen_t client_addr_len = sizeof(TCP_param_2->client_addr);
        
        char internal_buffer[200];
        
        // Poll struct params 
        fds_poll[0].fd = TCP_param_2->server_fd;
        fds_poll[0].events = POLLIN | POLLOUT;

        while(1)
        {
            int ready_act = poll(fds_poll, 1, -1);
            
            if (ready_act == -1) 
            {
                perror("Error in poll");
                return TCP_FAIL;
            }

            if (fds_poll[0].revents & POLLIN)
            {

                // accept 
                if ((TCP_param_2->client_fd = accept(TCP_param_2->server_fd, (struct sockaddr *) (&TCP_param_2->client_addr), &client_addr_len)) == -1) 
                {
                    perror("Acceptance failed");
                    return TCP_FAIL;
                }

                printf("[+] TCP : Accepted connection from client_fd : %d \n", TCP_param_2->client_fd);
                    
                if (tx_buffer != NULL)
                {
                    if (send(TCP_param_2->client_fd, tx_buffer, sizeof(tx_buffer),0) < 0)
                    {
                        perror("SEND_FAILURE \n");
                        return TCP_FAIL;
                    }
                }

                if (rx_buffer != NULL)
                {
                    
                    if (read(TCP_param_2->client_fd, internal_buffer, sizeof(internal_buffer)) < 0)
                    {
                        perror("RECV_FAILURE \n");
                        return TCP_FAIL;
                    }

                    memcpy(rx_buffer, internal_buffer, strlen(internal_buffer) + 1);

                    printf("[+] TCP : message in the buffer : %s \n", (char*) rx_buffer);

                        
                    // Writing to SHM
                    if (SHM_name != NULL)
                    {    
                        if (SHM_init_write(SHM_name, SHM_SIZE, mapping_ptr_1, rx_buffer) < 0)
                        {
                            perror("SHM Failure \n");
                            return TCP_FAIL;
                        }
                    }
                    
                    memset(internal_buffer, 0,sizeof(internal_buffer));
                    
                    memset(rx_buffer, 0, sizeof(rx_buffer));

                }
            }
        }
            
        // Socket closing
        printf("[+] TCP : server socket closed \n");

        close(TCP_param_2->server_fd);
        close(TCP_param_2->client_fd);
    }
    
    // For TCP_client() usage
    else if( (TCP_param_2->tcp_flag != 0) )
    {       
        // Poll struct params 
        fds_poll[0].fd = TCP_param_2->new_sock_fd;
        fds_poll[0].events = POLLIN | POLLOUT;
            
        while(1)    
        {   
            int ready_act = poll(fds_poll, 1, -1);
            
            if (ready_act == -1) 
            {
                perror("Error in poll");
                return TCP_FAIL;
            }

            if (fds_poll[0].revents & POLLIN)
            {
                if (tx_buffer != NULL)
                {
                    if (send(TCP_param_2->new_sock_fd, tx_buffer, sizeof(tx_buffer),0) < 0)
                    {
                        perror("SEND_FAILURE \n");
                        return TCP_FAIL;
                    }
                }

                if (rx_buffer != NULL)
                {
                    if (read(TCP_param_2->new_sock_fd, rx_buffer, sizeof(rx_buffer)) < 0)
                    {
                        perror("RECV_FAILURE \n");
                        return TCP_FAIL;
                    }

                    printf("[+] TCP : RX-buffer : %s \n", (char*) rx_buffer);
                }
            }
        }

        // Socket closing 
        printf("[+] TCP : client socket closed \n");

        close(TCP_param_2->new_sock_fd);  

    } 

    return TCP_PASS;
}


// Only use with TCP_server()
TCP_CHECK TCP_send_recv_update(TCP_param_t *TCP_param_3, void* tx_buffer, void* rx_buffer, char* SHM_name)
{
    // poll struct
    struct pollfd fds_poll[1];
    memset(fds_poll, 0, sizeof(fds_poll));

    // SHM parameters 
    size_t SHM_SIZE = 1000;
    char* mapping_ptr_1;

    // other buffers
    char internal_buff[200]; 
    char TCP_tmp_buff[200];
    char SHM_check_buff[200];

    if (TCP_param_3->tcp_flag == 0)
    {   
        socklen_t client_addr_len = sizeof(TCP_param_3->client_addr);

        // Poll struct params 
        fds_poll[0].fd = TCP_param_3->server_fd;
        fds_poll[0].events = POLLIN | POLLOUT;

        // Super - loop start 
        while (1)
        {   
            TCP_param_t TCP_param_client;

            int ready = poll(fds_poll, 1, -1);

            if (ready == -1) 
            {
                perror("Error in poll");
                exit(EXIT_FAILURE);
            }

            if (fds_poll[0].revents & POLLIN) 
            {
                
                // New connection is accepted  
                if ((TCP_param_3->client_fd = accept(TCP_param_3->server_fd, (struct sockaddr*)& TCP_param_3->client_addr, &client_addr_len)) == -1) 
                {
                    perror("Error accepting connection");
                    return TCP_FAIL;
                }

                printf("[+] TCP : Accepted connection from client_fd : %d \n", TCP_param_3->client_fd);
            
                if(rx_buffer != NULL)
                {
                    // Reading from client socket at particular PORT 
                    if ((read(TCP_param_3->client_fd, internal_buff, sizeof(internal_buff))) < 0)
                    {
                        perror("Server Disconnected");
                        return TCP_FAIL;
                    }

                    memcpy(rx_buffer, internal_buff, strlen(internal_buff) + 1);

                    memcpy(TCP_tmp_buff, internal_buff, strlen(internal_buff) + 1);

                    // Tokenizing and getting the status value 

                    int disc_msg_status = discmesgcheck(SHM_name, internal_buff, SHM_check_buff);

                    printf("status message %d \n", disc_msg_status);
                    
                    // Sending to the 'status_reply' to the same client socket

                    if (disc_msg_status == ALL_PASS || disc_msg_status == NOT_OK)
                    {
                        if ((send(TCP_param_3->client_fd, "4", sizeof("4"), 0)) == -1)
                        {
                            printf("TCP send failure");
                            return TCP_FAIL;
                        }

                        SHM_init_write(SHM_name, SHM_SIZE, mapping_ptr_1, rx_buffer);

                        for (int i = 0; i < MAX_SYNC_SERV ; i++)
                        {
                            if (TCP_client(&TCP_param_client, SYNC_CLIENT_PORTS[i], SYNC_CLIENT_IPS[i]) > 0)
                            {
                                TCP_send_recv(&TCP_param_client, TCP_tmp_buff, NULL);
                            }
                        }
                    }

                    else if (disc_msg_status == STAT_FAIL || disc_msg_status == PORT_FAIL)
                    {
                        if (disc_msg_status == STAT_FAIL)
                        {
                            if ((send(TCP_param_3->client_fd, "2", sizeof("2"), 0)) < 0)
                            {
                                printf("TCP send failure");
                                return TCP_FAIL;
                            }
                        }
                        
                        else if (disc_msg_status == PORT_FAIL)
                        {
                            if ((send(TCP_param_3->client_fd, "3", sizeof("3"), 0)) < 0)
                            {
                                printf("TCP send failure");
                                return TCP_FAIL;
                            }
                        }   
                    }

                    // Buffer Clearing 
                    
                    memset(internal_buff, 0, sizeof(internal_buff));

                    memset(rx_buffer, 0, sizeof(rx_buffer));

                    memset(TCP_tmp_buff, 0, sizeof(TCP_tmp_buff));

                    // SHM_del(SHM_name);
                }

            }

        }

        close(TCP_param_3->client_fd);
        close(TCP_param_3->server_fd);

        return TCP_PASS;
    }

} 