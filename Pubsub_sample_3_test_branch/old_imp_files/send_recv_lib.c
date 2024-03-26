// send and receive functions used by the TCP functions (TCP_client() and TCP_server())


#include "send_recv_lib.h"
#include "SHM_lib.h"
// #include "TCP_lib_new.h"

// for send and receive on a TCP socket 
FUNC_CHECK TCP_send_recv(TCP_param_t *TCP_param_2, void *tx_buffer, void* rx_buffer)
{   
    
    // For TCP server usage
    if( (TCP_param_2->tcp_flag == 0) )
    {
        socklen_t client_addr_len = sizeof(TCP_param_2->client_addr); 

        // accept 
        if ((TCP_param_2->client_fd = accept(TCP_param_2->server_fd, (struct sockaddr *) (&TCP_param_2->client_addr), &client_addr_len)) == -1) 
        {
            perror("Acceptance failed");
            return FUNC_FAIL;
        }

        printf("[+] TCP : Accepted connection client_fd : %d \n", TCP_param_2->client_fd);
            
        if (tx_buffer != NULL)
        {
            if (send(TCP_param_2->client_fd, tx_buffer, sizeof(tx_buffer),0) < 0)
            {
                perror("SEND_FAILURE \n");
                return FUNC_FAIL;
            }
        }

        if (rx_buffer != NULL)
        {
            if (read(TCP_param_2->client_fd, rx_buffer, sizeof(rx_buffer)) < 0)
            {
                perror("RECV_FAILURE \n");
                return FUNC_FAIL;
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
            if (send(TCP_param_2->new_sock_fd, tx_buffer, sizeof(tx_buffer),0) < 0)
            {
                perror("SEND_FAILURE \n");
                return FUNC_FAIL;
            }
        }

        if (rx_buffer != NULL)
        {
            if (read(TCP_param_2->new_sock_fd, rx_buffer, sizeof(rx_buffer)) < 0)
            {
                perror("RECV_FAILURE \n");
                return FUNC_FAIL;
            }
        }

        // Socket closing 
        printf("[+] TCP : client socket closed \n");

        close(TCP_param_2->new_sock_fd); 
    } 

    return FUNC_PASS;
}

// for send and receive on a TCP socket in polling mode.
// Will be writing in SHM 
FUNC_CHECK TCP_send_recv_poll(TCP_param_t *TCP_param_2, void *tx_buffer, void* rx_buffer, char* SHM_name)
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
                exit(EXIT_FAILURE);
            }

            if (fds_poll[0].revents & POLLIN)
            {

                // accept 
                if ((TCP_param_2->client_fd = accept(TCP_param_2->server_fd, (struct sockaddr *) (&TCP_param_2->client_addr), &client_addr_len)) == -1) 
                {
                    perror("Acceptance failed");
                    return FUNC_FAIL;
                }

                printf("[+] TCP : Accepted connection from client_fd : %d \n", TCP_param_2->client_fd);
                    
                if (tx_buffer != NULL)
                {
                    if (send(TCP_param_2->client_fd, tx_buffer, sizeof(tx_buffer),0) < 0)
                    {
                        perror("SEND_FAILURE \n");
                        return FUNC_FAIL;
                    }
                }

                if (rx_buffer != NULL)
                {
                    
                    if (read(TCP_param_2->client_fd, internal_buffer, sizeof(internal_buffer)) < 0)
                    {
                        perror("RECV_FAILURE \n");
                        return FUNC_FAIL;
                    }

                    memcpy(rx_buffer, internal_buffer, strlen(internal_buffer) + 1);

                    printf("[+] TCP : message in the buffer : %s \n", (char*) rx_buffer);

                        
                    // Writing to SHM
                    if (SHM_name != NULL)
                    {    
                        if (SHM_init_write(SHM_name, SHM_SIZE, mapping_ptr_1, rx_buffer) < 0)
                        {
                            perror("SHM Failure \n");
                            return FUNC_FAIL;
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
                exit(EXIT_FAILURE);
            }

            if (fds_poll[0].revents & POLLIN)
            {
                if (tx_buffer != NULL)
                {
                    if (send(TCP_param_2->new_sock_fd, tx_buffer, sizeof(tx_buffer),0) < 0)
                    {
                        perror("SEND_FAILURE \n");
                        return FUNC_FAIL;
                    }
                }

                if (rx_buffer != NULL)
                {
                    if (read(TCP_param_2->new_sock_fd, rx_buffer, sizeof(rx_buffer)) < 0)
                    {
                        perror("RECV_FAILURE \n");
                        return FUNC_FAIL;
                    }

                    printf("[+] TCP : RX-buffer : %s \n", (char*) rx_buffer);
                }
            }
        }

        // Socket closing 
        printf("[+] TCP : client socket closed \n");

        close(TCP_param_2->new_sock_fd);  

    } 

    return FUNC_PASS;
}


