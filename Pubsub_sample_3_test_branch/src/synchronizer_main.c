// Private includes
// TCP_new_lib.h implementation  
#include "TCP_lib.h"
#include "SHM_lib.h"
#include <string.h>
#include <pthread.h>

#include "../config.h"

// #define SYNC_PROCESS
// #define MAX_CLIENTS_SYNC 5

// User defined functions 

// Global variables 
char TCP_rx_buff[40]; // Receive buffer for all the internal process 
char TCP_rx_buff_1[40]; // Receive buffer for all the external connections 
TCP_param_t TCP_1; // Internal Server obj.
TCP_param_t TCP_2; // External Server obj.

// Thread functions 


// Internal Server listen port - 8010
void* sync_proc_1_func(void* arg)
{  
//    usleep(500);
    if (TCP_server(&TCP_1, SYNC_PORT_INT, 5) > 0 ) // Sync Server Internal (Writing in a common shared memory in same device)
    {
        TCP_send_recv_update(&TCP_1, NULL, TCP_rx_buff, SHM_NAME_SYNC);
    }
}

// External TCP process 
void* sync_proc_2_func(void* arg)
{
    if (TCP_server(&TCP_2, SYNC_PORT_EXT, 5) > 0 ) // Sync Server Internal (Writing in a common shared memory in same device)
    {
        TCP_send_recv_poll(&TCP_2, NULL, TCP_rx_buff_1, SHM_NAME_SYNC);
    }
}

int main()
{
    pthread_t sync_proc_1,sync_proc_2; 
    
    // Create thread 2
    if (pthread_create(&sync_proc_1, NULL, sync_proc_1_func, NULL) != 0) 
    {
        perror("Error creating thread 1");
        exit(EXIT_FAILURE);
    }


    // Creating thread 2
    if (pthread_create(&sync_proc_2, NULL, sync_proc_2_func, NULL) != 0) 
    {
        perror("Error creating thread 2");
        exit(EXIT_FAILURE);
    }
    
    pthread_join(sync_proc_1, NULL);
    pthread_join(sync_proc_2, NULL);

    // pthread_mutex_destroy(&mutex);

    return 0;

}