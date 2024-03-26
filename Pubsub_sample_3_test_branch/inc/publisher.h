// Declaration of publisher

#include <string.h>
#include <stdio.h>

typedef enum 
{
    PUB_FAIL = -1,
    PUB_PASS = 1,
} PUB_CHECK;

/// @brief - Publisher process that takes arguements as the parameters of Disc. Msg 
/// @param - char * - Topic string
/// @param - char * - IP_address needed 
/// @param - char * - port needed
/// @param - char * - Respective Synchronizer PORT  
/// @param - char * - DISC-MESSAGE buffer 
/// @return - (Enum) PUB_CHECK - PUB_PASS/PUB_FAIL
PUB_CHECK publisher_init(char* , char* , char* , char* , char* );

/// @brief - main publisher function to be run by all the publisher process 
/// @param - char * - Port of the respective client port to listen to 
/// @param - char * - TCP Transmit buffer 
/// @return - (Enum) PUB_CHECK - PUB_PASS/PUB_FAIL 
PUB_CHECK publisher_proc(char* , char*);

/// @brief - Once the TCP is finished transmission means kill the topic and send message to sync.
/// @param - The SYNCHRONIZER PORT
/// @param - DISC-MESSAGE buffer 
/// @return - (Enum) PUB_CHECK - PUB_PASS/PUB_FAIL
PUB_CHECK publisher_kill(char* , char* );
