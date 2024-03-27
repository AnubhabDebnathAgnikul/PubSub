// New implementation contatining a completely new approach

// mutex should be implemented 

// function 1 

// Headers
#include "TCP_lib.h"
#include "GenDiscMsg.h"
#include "string_lib.h"

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


// Function to be run by the thread
int server_thread(void* args) 
{
    topic_param_t* topic_1 = (topic_param_t*) args;

    TCP_param_t TCP_1;

    while(1)
    {
        
        
        if (atomic_load(&(topic_1->trig_sig)) == 1)
        {
            TCP_server(&TCP_1, topic_1->port_str, 5);
            
            printf("Trig_signal received \n");

            printf("message to be send %s", (char*) topic_1->actual_message);

           
            TCP_send_recv(&TCP_1, topic_1->actual_message, NULL);
                
            atomic_store(&(topic_1->trig_sig),0); // changing back to 0 state 

            free(topic_1->actual_message);
                    
        }

         // Kill signal triggering 
            if(atomic_load(&(topic_1->kill_sig)) == 1)
            {
                atomic_store(&(topic_1->trig_sig),3);
                        
                printf("Kill_signal received \n");

                free(topic_1);

                pthread_exit(NULL);

                atomic_store(&(topic_1->kill_sig),0);
            }
    }
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
            // memset(Discmsg_buff, 0, sizeof(Discmsg_buff));
    
            if (rx_buff_value == 3) 
            {
                printf("PORT_CLASH DETECTED \n");
                exit(EXIT_FAILURE);
            }
            
            else if (rx_buff_value == 4 || rx_buff_value == 5)
            {
                printf("Thread created \n");

                atomic_store(&(topic_1->trig_sig),0);
                
                pthread_create(&topic_1->server_id, NULL, (void*) server_thread, topic_1);

                pthread_detach(topic_1->server_id);

                return topic_1;
            }
        }
    }

}

// 2nd function 
// just change the topic_1->trig_sig  
PUB_CHECK publisher_proc(void* args, void* tx_buffer)
{
    topic_param_t* topic_1 = (topic_param_t*) args;

    topic_1->actual_message = malloc(sizeof(tx_buffer));

    if (atomic_load(&(topic_1->trig_sig)) != 0)
    {
        return -1;                   
    }

    printf("copied to memory \n");
                
    memcpy(topic_1->actual_message, tx_buffer, sizeof(tx_buffer));

    atomic_store(&(topic_1->trig_sig), 1);

    while(1)
    {
        if(atomic_load(&(topic_1->trig_sig)) == 0)
        {
            return PUB_PASS;
        }
    }

}

// 3rd function 
// still implementation '/0' pending 
// kill signal integrated 
PUB_CHECK publisher_kill(void* args, char* sync_port, char* disc_msg_buffer)
{
    topic_param_t *topic_1 = (topic_param_t*) args;

    TCP_param_t TCP_C2;
    

    if (atomic_load(&(topic_1->kill_sig)) == 0)
    {   
        if (TCP_client(&TCP_C2, sync_port, "127.0.0.1") > 0)
        {
            
            updatesubstr(disc_msg_buffer, "1", "0");
            
            TCP_send_recv(&TCP_C2, disc_msg_buffer, NULL);

            memset(disc_msg_buffer, 0, sizeof(disc_msg_buffer));

        }

        atomic_store(&(topic_1->kill_sig),1); // kill_flag is ON

    }

    // while(1)
    // {
    //     if(atomic_load(&(topic_1->kill_sig)) == 0)
    //     {
    //         return PUB_PASS;
    //     }
    // }

    return PUB_PASS;


}

int main()
{
    topic_param_t T1;
    topic_param_t T2;
    
    char tx_buffer_1[] = "GOOGLE";
    char tx_buffer_2[] = "BOOK";
    char tx_buffer_3[] = "YOUTUBE";
    char tx_buffer_4[] = "TOWO";
    
    char disc_msg_1[40];
    char disc_msg_2[40];

    // Things to add 
    // 1. Adding Kill signal topic status '/0' in publisher_kill() 
    // 2. Test by creating multiple threads.
    // 3. Memory freeing and memory allocation 
    // 4. Testing with actual TCP_lib.c functions.
    // 5. Publisher Code cleanup. 
    
    topic_param_t* TOPIC_1 = publisher_init("FLT", "1.1.1.35", "8030", "8050", disc_msg_1);

    // topic_param_t* TOPIC_2 = publisher_init("NAV", "1.1.1.45", "8031", "8051", disc_msg_2);

    publisher_proc(TOPIC_1, tx_buffer_1);

    // sleep(1);

    publisher_proc(TOPIC_1, tx_buffer_2);

    // sleep(1);    

    publisher_proc(TOPIC_1, tx_buffer_3);

    // sleep(1);    

    // publisher_proc(TOPIC_1, tx_buffer_4);

    // sleep(1);

    publisher_kill(TOPIC_1, "8050", disc_msg_1);

    // sleep(5);

    topic_param_t* TOPIC_2 = publisher_init("NAV", "1.1.1.45", "8031", "8051", disc_msg_2);

    // sleep(2); 

    publisher_proc(TOPIC_2, tx_buffer_4);

    publisher_kill(TOPIC_2, "8051", disc_msg_2);

    // publisher_kill(TOPIC_2, "8051", disc_msg_2);

    // sleep(2);

    // publisher_proc(TOPIC_1, tx_buffer_1);

    // publisher_proc(TOPIC_1, tx_buffer_1);

    return 0;

}