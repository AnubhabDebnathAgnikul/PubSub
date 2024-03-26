// Common - Process (Pub/Sub)

// Create TCP server or Create a TCP client 

// Includes 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>

// User enum or structs

// Enum for checking 
typedef enum
{
    FAIL = -1, 
    PASS = 1,  
} STATUS_CHECK;


/// @brief - TCP Server to connect that continuosly listen in specified port and send data to all the clients connected to PORT  
/// @param - PORT .no to listen in.. 
/// @param - Global Transmit_buffer (String-Only) Update in nxt version
/// @return - (Enum) STATUS_CHECK 
STATUS_CHECK TCP_server(char* , char* );

/// @brief - TCP server func. to continuosly poll and send to a TCP server  
/// @param - PORT .no to listen to 
/// @param - Global Receive_buffer (String-Only) Update in nxt version
/// @param - Name of SHM to write 
/// @return - (Enum) STATUS_CHECK 
STATUS_CHECK TCP_server_poll_update(char* ,char*, char*); // Only write to a TCP client


// /// @brief - Driver function of the TCP_server_poll() (Pub-side usage only) 
// /// @param - char* - tx_buff 
// /// @return - (ENUM) STATUS_CHECK
// STATUS_CHECK trig_func(char* tx_buff);

/// @brief - TCP server func. to continuosly poll and write to SHM buff  
/// @param - PORT .no to listen to
/// @param - Global Transmit_buffer (String-Only)
/// @param - Global Receive_buffer (String-Only) Update in nxt version
/// @param - Name of SHM to write 
/// @return - (Enum) STATUS_CHECK Flag 
STATUS_CHECK TCP_server_poll(char*, char*, char* , int);

/// @brief - TCP client func. for single message send and close 
/// @param - PORT no. 
/// @param - IP_ADDR to connect to 
/// @param - Global TCP Transmit Buffer (String-Only) Update in next version 
/// @param - Global TCP Receive Buffer 
/// @return - (Enum) STATUS_CHECK Flag 
STATUS_CHECK TCP_Client(char* ,char* , char* ,char* );


/// @brief - TCP client for continous message receiving from server (while)
/// @param - PORT no. 
/// @param - IP_ADDR to connect to 
/// @param - Global Receive Buffer (String-Only) Update in next version 
/// @return - (Enum) STATUS_CHECK Flag 
STATUS_CHECK TCP_Client_Cont(char* ,char* , char*);



