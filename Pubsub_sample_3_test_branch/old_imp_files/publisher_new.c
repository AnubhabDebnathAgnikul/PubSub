// New implementation of Publisher process 
// contains 3 functions 
// 1. publisher_init() 
// 2. publisher_proc()
// 3. publisher_kill()

// Private Headers 
#include "publisher.h"
#include "GenDiscMsg.h"
#include "TCP_lib.h"
#include "SHM_lib.h"
#include "string_lib.h"    
#include "../config.h"

// test_packet_t test_buff;

PUB_CHECK publisher_init(char* topic_str, char* IP_need_str, char* port_need_str, char* sync_port, char* gendisc_buff)
{  
    // Local variables
    char TCP_rx_buff[5];
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

PUB_CHECK publisher_proc(char* client_port, char* test_buff)
{
    int TCP_stat = TCP_server(client_port, test_buff);

    if (TCP_stat > 0)
    {
        return PUB_PASS;
    }

    else if (TCP_stat < 0)
    {
        return PUB_FAIL;
    }

    // Memory clearing area

}

PUB_CHECK publisher_kill(char* sync_port, char* disc_msg)
{
    updatesubstr(disc_msg, "1", "0");

    printf("UPDATED DISC MESG : %s \n", disc_msg);
        
    TCP_Client(sync_port, "127.0.0.1", disc_msg, NULL);

    return PUB_PASS;

}