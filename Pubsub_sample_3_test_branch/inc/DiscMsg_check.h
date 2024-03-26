#include <stdio.h>

typedef enum 
{
    // Checks 
    STAT_FAIL = 2,
    PORT_FAIL = 3,
    ALL_PASS = 4,
    NOT_OK = -1,
    
    // FUNC SUCESS OR FAIL 
    
} DISC_CHECK;

/// @brief - Function to check for criteria of the discovery message 
/// @param - char * - SHM name 
/// @param - char * - Transmit buffer of TCP 
/// @param - char * - Value read from the SHM 
/// @return - (Enum) DISC_CHECK  
DISC_CHECK discmesgcheck(char* , char* , char* );
