// Integrating with the function 

#include "TCP_lib.h"
#include "SHM_lib.h"
#include "pthread.h"
#include "DiscMsg_check.h"
#include "string_lib.h"

char tx_message[200];
#define MAX_CLIENTS 5

int trig_func_thread()
{
    trig_func(tx_message);
}

int TCP_func_thread()
{   
    // running continuos poll here 
    TCP_server_poll("8030", tx_message, NULL, NULL, MAX_CLIENTS);      

}

int main()
{
    pthread_t thread_1,thread_2;
    pthread_create(&thread_1, NULL, (void *) trig_func_thread , NULL);
    pthread_create(&thread_2, NULL, (void *) TCP_func_thread, NULL);

    // Join threads

    // Exit threads 
    pthread_exit(NULL); 
}    