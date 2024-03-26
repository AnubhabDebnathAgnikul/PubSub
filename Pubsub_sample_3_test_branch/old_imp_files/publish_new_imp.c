#include "TCP_lib.h"
#include "SHM_lib.h"
#include "string_lib.h"
#include "DiscMsg_check.h"
#include "GenDiscMsg.h"
#include "pthread.h"
#include "../config.h"

int trig_value = 0;
// char tcp_buff[200];

// Enum 
typedef enum 
{
    PUB_FAIL = -1,
    PUB_PASS = 1,
} PUB_CHECK;

// trig function 
void* trig_func(char* tx_buff)
{
    while(1)
    {
        printf("enter message to send to clients : \n");
        fgets(tx_buff, sizeof(tx_buff), stdin);
        trig_value = 1;
    }
} 

// TCP_server_pub() 

STATUS_CHECK TCP_server_poll_pub(char* port_msg, char* g_tx_buff)
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    struct pollfd fds_poll[1];
    socklen_t client_addr_len = sizeof(client_addr);
    int port_int;
    int reuse = 1;

    // char internal_buff[2500];

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
    fds_poll[0].events = POLLIN | POLLOUT;

    // Super - loop start 
    while (1)
    {  

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

            printf("New client connected, socket fd is %d \n", client_socket);

            char input[5];
            printf("Enter number to select \n");
            printf("1. Send topic once \n");
            printf("2. Send topic continuos \n");
            fgets(input, sizeof(input),stdin);

            // 
            int input_int = atoi(input);

            if (input_int == 1)
            {
            // Reading from client socket at particular PORT 
            if ((send(client_socket, g_tx_buff, strlen(g_tx_buff),0)) < 0)
            {
                printf("Server Disconnected");
                return FAIL;
            }

            input_int = 0;

            printf("Message transmitted to all sockets %s", g_tx_buff);

            }

            if (input_int == 2)
            {   
                for (int count = 0; count < 10000 ; count++)
                {
                    // Reading from client socket at particular PORT 
                    if ((send(client_socket, g_tx_buff, strlen(g_tx_buff),0)) < 0)
                    {
                        printf("Server Disconnected");
                        return FAIL;
                    }

                    input_int = 0;

                    printf("Message transmitted to all sockets %s", g_tx_buff);
                }
            }
            
        }

            // memcpy(g_tx_buff, internal_buff, strlen(internal_buff) + 1);

            // printf("message in the buffer : %s \n",g_tx_buff);

            // SHM_init_write(SHM_name, SHM_SIZE, mapping_ptr_1, g_tx_buff);

            // memset(internal_buff,0,sizeof(internal_buff));

            // memset(g_tx_buff,0,sizeof(g_tx_buff));
    
    }

    // Closing both client and server sockets
    // Note : Will never ever reach here , continuosly in listening mode 
    close(client_socket);
    close(server_socket);

    return PASS;

}


// #define MAX_CLIENTS 5

int publisher_init(char* topic_str, char* IP_need_str, char* port_need_str, char* sync_port, char* gendisc_buff)
{  
    // Local variables
    char TCP_rx_buff[20];
    int rx_buff_value; 

    GenDiscMsg(topic_str, IP_need_str, port_need_str, gendisc_buff);

    printf("Disc_message %s \n", gendisc_buff);
 
    // Make TCP Client to send and receive data 
    if (TCP_Client(sync_port, OWN_IP, gendisc_buff, TCP_rx_buff) > 0) // Creating a TCP_Client to connect to Synchronizer
    {
        rx_buff_value = atoi(TCP_rx_buff);
        printf("buff int : %d \n", rx_buff_value);
   
        if (rx_buff_value == 3) 
        {
            printf("PORT_CLASH DETECTED \n");
            return PUB_FAIL;
        }
        
        else if(rx_buff_value == 4 || rx_buff_value == 5)
        {
            return PUB_PASS;
        }

    }

    // Memory clearing area 

}

// thread functions 

int publisher_proc(char* client_port, char* tx_buff_1)
{   
    pthread_t thread_id;

    int MAX_CLIENTS = 5;

    printf("New Pub Thread created \n");
        
    pthread_create(&thread_id, NULL, (void*) (TCP_server_poll_pub(client_port, tx_buff_1)) , NULL);
    
    pthread_exit(NULL); 
   
}

int main()
{
    char Disc_msg_buff[40]; // Disc Message buffer

    int init_check = publisher_init(TOPIC_PUB, IP_ADDR_NEED_PUB, PORT_NEED_PUB, SYNC_PORT_PUB,Disc_msg_buff); // Init check 

    if (init_check > 0)
    {   
        publisher_proc(PORT_NEED_PUB, TCP_TX_MSG_BUFF);

    }

    return 0;

}