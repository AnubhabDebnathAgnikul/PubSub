#include "SHM_lib.h" 
#include "string_lib.h"
#include "DiscMsg_check.h"

// Old functions

DISC_CHECK discmesgcheck(char *shm_name, char* tx_buffer, char *rx_buffer)
{
    size_t shm_size = 1000;
    void* map_ptr;
    
    // Port buffer to compare 
    
    // port_buff 
    char port_buff_tx[20];
    char port_buff_rx[20];

    // Status_buff
    char status_buff_tx[3];
    char status_buff_rx[3];

    if (SHM_read_close(shm_name, shm_size, map_ptr, rx_buffer) == -1)
    {
        perror("SHM Read Error \n");
        return NOT_OK;
    }

    // Detokenize both RX and TX buffers 
    str_tokenize(tx_buffer, port_buff_tx, NULL , status_buff_tx);
    str_tokenize(rx_buffer, port_buff_tx, NULL , status_buff_rx);

    if ((strcmp(status_buff_rx , status_buff_tx) != 0))
    {
       return ALL_PASS;
    }

    else 
    {  

        if (strcmp(port_buff_tx, port_buff_tx) == 0)
        {
            return PORT_FAIL;
        } 

        else
        {
            return ALL_PASS;
        }
    }

}