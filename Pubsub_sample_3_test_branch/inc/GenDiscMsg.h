// Pub - Process

// Header includes
#include <stdio.h>
#include <string.h>

// User headers

#define OWN_IP "127.0.0.1" //IP address of the system (DEFINE)

/// @brief - Generate the discovery message by getting the parameters from user 
/// @param - char* - Topic to subscribe 
/// @param - char* - IP needed
/// @param - char* - Port_need
/// @param - char* - char buffer of disc message 
/// @return - (void) NIL 
void GenDiscMsg(char *, char *, char *, char *);

