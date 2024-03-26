// inlcudes 

#include "TCP_lib_new.h"
// #include "send_recv_lib.h"

int main()
{   
    TCP_param_t TCP_1;
    char TCP_tx_buff[] = "google";
    char TCP_rx_buff[200];
    
    if (TCP_server(&TCP_1, "8030", 3) > 0)
    {
        TCP_send_recv_poll(&TCP_1, NULL, TCP_rx_buff, "/test_SHM");
    }
    
    return 0;
}