// Private inlcudes // Only sender config  

#include "publisher.h"
#include "stdint.h"
#include "../config.h"
#include "stdbool.h"

int main()
{   
    // Disc Message buffer
    char Disc_msg_buff[40]; 

    int init_check = publisher_init(TOPIC_PUB, IP_ADDR_NEED_PUB, PORT_NEED_PUB, SYNC_PORT_PUB,Disc_msg_buff);

    if (init_check > 0)
    {
        int proc_check = publisher_proc(PORT_NEED_PUB, TCP_TX_MSG_BUFF);

        if (proc_check > 0)
        {
            publisher_kill(SYNC_PORT_PUB, Disc_msg_buff);
        }
        else if (proc_check < 0)
        {
            printf(" PUBLISHER_PROC ERROR \n");
        }

    }

    else if (init_check < 0) 
    {
        
        printf(" PUBLISHER INIT ERROR \n");
    
    }

    // Memory clearing area 
    memset(Disc_msg_buff, 0, sizeof(Disc_msg_buff));

    return 0;

}

