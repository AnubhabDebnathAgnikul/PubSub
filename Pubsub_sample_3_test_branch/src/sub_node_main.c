#include "subscriber.h" // Only receiver config. 
#include "../config.h"

int main()
{
    char TCP_RX_BUFF[200];
    
    subscriber_proc(SHM_NAME_SUB, TCP_RX_BUFF, PORT_CHECK_SUB);

    return 0;

}