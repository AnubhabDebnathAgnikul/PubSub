// Version - V1.2 
// Old model of - PUBLISHER IMPLEMENTATION 
// Main pub process to be run by the Publisher 
// Publisher only Process

// Private includes 
#include "GenDiscMsg.h"
#include "TCP_lib.h"
#include "SHM_lib.h"
#include "string_lib.h"
#include "publisher.h"

#define MAX_MSG_SIZE 100

// int publish_init();

int publisher_proc(char* sub_topic,char* IP_need,char* port_need, char* TCP_tx_buff, char* sync_port) 
{
    
    // char Buffer to store respective info 
    char token_port[10];
    char read_buff[200];
    char Disc_msg_buff[MAX_MSG_SIZE]; 

    // Step - 1 - Discovery message generation 
    GenDiscMsg(sub_topic, IP_need, port_need, Disc_msg_buff);

    memcpy(read_buff, Disc_msg_buff, strlen(Disc_msg_buff) + 1);

    // Step - 2 - Form a TCP client with the TCP sync process and send the message
    TCP_Client(sync_port,"127.0.0.1", Disc_msg_buff);

    // Step - 3 - Tokenize the Disc_msg_buff and get the port_need string from the Disc_message 
    str_tokenize(Disc_msg_buff, token_port, NULL, NULL);

    printf("port no: %s \n", token_port);

    // usleep(200); // User Delay for synchronization 

    // Step - 4 form a TCP_Client that connects to server listening on same port and continuosly sends message    
    int tcp_stat = TCP_server(token_port, TCP_tx_buff);
    
    // Step - 5 - Updating the string and changing the status to "/0" after the data transmission is over 
    updatesubstr(read_buff, "1", "0"); //Step - 4

    // Only send the "/0" status when Message transmission is finished. 
    if (tcp_stat == PASS)
    {  
        // Display the Updated message 
        printf("updated substring after TCP conn. is : %s \n", read_buff);

        // Step - 6 - Send the updated message to the respective synchronizer with sync-port 
        TCP_Client(sync_port, "127.0.0.1", read_buff); 
    
    }
        // Step - 7 (Optional step included) Clearing the memory of all the buffers invloved after all operations are complete memset()
        // All the temproary buffers should be cleared here 

    memset(read_buff,0, sizeof(read_buff)); // read_buff
    memset(Disc_msg_buff, 0, sizeof(Disc_msg_buff)); // Disc_msg_buff
    memset(token_port, 0, sizeof(token_port)); // token_port

    return 0;

}