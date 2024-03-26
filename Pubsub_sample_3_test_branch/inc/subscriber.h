#include <stdio.h>

/// @brief - Function that suscriber process will be running (Super - loop (while)) 
/// @param - char * - Name of the SHM instance
/// @param - char * - TCP buffer (char array for testing)
/// @param - char * - Port string to be checked 
/// @return - int Function status 
int subscriber_proc(char*, char*, char*);