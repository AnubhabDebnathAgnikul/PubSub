// Pub - process
// contains function definition to to generate a DiscMessage
// Function to write the message to SHM
  
// #define OWN_IP "1.1.1.85"
#define MAX_SIZE_LEN 300

#include "GenDiscMsg.h"
#include "TCP_lib.h"

char result_msg[] = {0};

void GenDiscMsg(char* subscribe_topic,char* IP_need,char* Port_need, char* result_msg)
{   
    
    strncat(result_msg,subscribe_topic,sizeof(subscribe_topic));
    strncat(result_msg , "/",sizeof("/"));
    strncat(result_msg,IP_need,sizeof(IP_need));
    strncat(result_msg,"/",sizeof("/"));
    strncat(result_msg,Port_need,sizeof(Port_need));
    strncat(result_msg,"/",sizeof("/"));
    strncat(result_msg,OWN_IP,sizeof(OWN_IP));
    strncat(result_msg,"/",sizeof("/"));
    strncat(result_msg,"1",sizeof("1"));
    strncat(result_msg,"\0",sizeof("\0"));

    printf("\n the result string is %s \n",result_msg);

    // TCP_Client("8010","127.0.0.1",result_msg); // form a TCP client to a internaL TCP PROC. and decode the message in main Publish proc. and form a TCP Server there 
    
    // Write the DiscMsg to the SHM 
    // SHM_init_write (shm_name,shm_size, map_ptr,result_msg);
} 