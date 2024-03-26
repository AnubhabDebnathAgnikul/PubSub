// New implementation contatining a completely new approach

// mutex should be implemented 

// function 1 

// Headers
#include "TCP_lib.h"
#include "GenDiscMsg.h"
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

#include "../config.h"

// enums (or) structs
typedef struct 
{
  
  atomic_int trig_sig; // trigger_signal atomic variable 
  atomic_int kill_sig; // kill_signal atomic variable 
  char topic[20];
  char port_str[10];
  void* actual_message;
  pthread_t server_id;
  pthread_mutex_t thread_mutex;
  int input_ready;

}topic_param_t;

char shared_variable[200];

typedef enum 
{
    PUB_PASS = 1,
    PUB_FAIL = -1
}PUB_CHECK;
 
int server_thread(void* args) 
{
    
    topic_param_t* topic_1 = (topic_param_t*) args;
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_sockets[5];
    socklen_t client_addr_len = sizeof(client_addr);
    int port_int;
    int reuse = 1;

    char internal_buff[2500];


    // Creating socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Type conversion string -> int
    port_int = atoi(topic_1->port_str);

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
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(server_socket, 5) == -1) 
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    
    printf("[+] TCP : TCP Server created and listening on Port ID: %d......\n", port_int);

    // accept 
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) 
    {
        perror("Acceptance failed");
        exit(EXIT_FAILURE);

    }

    printf("connections accepted from client_fd : %d \n", client_socket);

    while(1)
    {
        
        if (atomic_load(&(topic_1->trig_sig)) == 1)
        {
            printf("Trig_signal received \n");

            printf("Value send to client : %s \n", (char*) topic_1->actual_message);
            
            if (send(client_socket, topic_1->actual_message, sizeof(topic_1->actual_message), 0) < 0)
            {
                perror("SEND_FAILURE");
                exit(EXIT_FAILURE);
            }

            atomic_store(&(topic_1->trig_sig),2);

            // if(atomic_load(&(topic_1->kill_sig)) == 1)
            // {
            //     atomic_store(&(topic_1->trig_sig),3);
                
            //     printf("Kill_signal received \n");

            //     free(topic_1);

            //     pthread_exit(NULL);

            //     atomic_store(&(topic_1->kill_sig),0);

            //     break;
            // }
        
            // continue;
            
        }
        

        // break;
                        
        // printf("[+] TCP : Message sent to client successfully : Message = %s pkt count : %d\n",g_tx_buffer,i);
                    
    }
         

    // Closing both client and server sockets
    close(client_socket);
    close(server_socket);
    
    return 0;
} 

// just change the topic_1->trig_sig  
PUB_CHECK publisher_proc(void* args, void* tx_buffer)
{
    topic_param_t* topic_1 = (topic_param_t*) args;

    topic_1->actual_message = malloc(sizeof(tx_buffer));

    // while(1)
    // {   

        if (atomic_load(&(topic_1->trig_sig)) == 0)
        {
            printf("copied to memory \n");
                
            memcpy(topic_1->actual_message, tx_buffer, sizeof(tx_buffer));

            memset(tx_buffer, 0, sizeof(tx_buffer));

            atomic_store(&(topic_1->trig_sig), 1);
                
        }

        else if (atomic_load(&(topic_1->trig_sig)) == 2)
        {
            
            // printf("pthread_killed \n");

            atomic_store(&(topic_1->trig_sig),1);

            // pthread_exit(&topic_1->server_id);

            // free(topic_1);
            
            // break;
        }

        // else if (atomic_load(&topic_1->trig_sig) == 3)
        // {
            
        //     break;
        
        // }

        // break;
    // }

}

// 1st function 
topic_param_t* publisher_init(char* topic_str, char* ip_addr_str, char* port_str, char* sync_port,char* Discmsg_buff)
{
    // Local variables
    char TCP_rx_buff[10];
    int rx_buff_value;
    pthread_t server_id; // for thread
    TCP_param_t TCP_C1;

    // parameters. 
    topic_param_t *topic_1 = malloc(sizeof(topic_param_t));
    
    // 1. set the topic_param_t parameters
    // Trigger signal and kill signal assigned to 0 
    topic_1->trig_sig = 0; 
    topic_1->kill_sig = 0;
    // topic_1->input_ready = 0;
    // topic_1->server_th_id = server_id;
    strcpy(topic_1->topic, topic_str); 
    strcpy(topic_1->port_str, port_str);

    // 2. Gendisc_msg()
    GenDiscMsg(topic_str, ip_addr_str, port_str, Discmsg_buff);
    
    // 3. Sending DiscMsg to Sync server and checking its validity
    if (TCP_client(&TCP_C1, sync_port, "127.0.0.1") > 0) // Creating a TCP_Client to connect to Synchronizer
    {
        
        if (TCP_send_recv(&TCP_C1, Discmsg_buff, TCP_rx_buff) > 0)
        {
            rx_buff_value = atoi(TCP_rx_buff);
            printf("buff int : %d \n", rx_buff_value);
            memset(Discmsg_buff, 0, sizeof(Discmsg_buff));
    
            if (rx_buff_value == 3) 
            {
                printf("PORT_CLASH DETECTED \n");
                exit(EXIT_FAILURE);
            }
            
            else if (rx_buff_value == 4 || rx_buff_value == 5)
            {
                printf("Thread created \n");

                atomic_store(&(topic_1->trig_sig),0);

                pthread_mutex_init(&(topic_1->thread_mutex),NULL);
                
                pthread_create(&topic_1->server_id, NULL, (void*) server_thread, topic_1);

                pthread_detach(topic_1->server_id);

                return topic_1;
            }
        }
    }

}

// still implementation '/0' pending 
// kill signal integrate 
PUB_CHECK publisher_kill(void* args)
{
    topic_param_t *topic_1 = (topic_param_t*) args;
    
    // while(1)
    // {    
        // pthread_mutex_lock(&(topic_1->thread_mutex));

        if (atomic_load(&(topic_1->kill_sig)) == 0)
        {    
            
            atomic_store(&(topic_1->kill_sig),1); // kill_flag is ON
        
        }
        // free(topic_1);
        
        // pthread_mutex_unlock(&(topic_1->thread_mutex));

    // }
    // return PUB_PASS;
}

int main()
{
    topic_param_t T1;
    topic_param_t T2;
    
    char tx_buffer_1[] = "GOOGLE";
    char tx_buffer_2[] = "FACEBOOK";
    
    char disc_msg_1[50];
    char disc_msg_2[50];

    // 1. GendiscMsg 
    
    topic_param_t* TOPIC_1 = publisher_init("FLT", "1.1.1.35", "8030", "8050", disc_msg_1);

    publisher_proc(TOPIC_1, tx_buffer_1);

    // publisher_proc(TOPIC_1, tx_buffer_2);

    // topic_param_t* TOPIC_2 = publisher_init("NAV", "1.1.1.44", "8031", "8051", disc_msg_2);

    // publisher_init(&T1, "FLT", "1.1.1.35", "8030", "8050", disc_msg_1);
    // publisher_kill(TOPIC_1);

    // sleep(2);

    

    // publisher_proc(TOPIC_1, tx_buffer_2);

    // publisher_kill(TOPIC_1);

    // publisher_kill(&T1);
    
    // sleep(5);

    // publisher_init(&T2, "NAV", "1.1.1.45", "8031", "8051", disc_msg_2);

    return 0;
}