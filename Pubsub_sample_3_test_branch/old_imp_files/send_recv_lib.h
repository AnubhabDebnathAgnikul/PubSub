// Contains function Declaration of TCP_send_recv() functions 

// #include "TCP_lib_new.h"

// Private Enum (or) structs
// Enum for checking 

// Enum for checking 
typedef enum
{
    FUNC_FAIL = -1, 
    FUNC_PASS = 1,  
} FUNC_CHECK;

typedef struct 
{   
    int tcp_flag; // '0' for server and '1' for client
    int server_fd; // Server File Descriptor 
    int client_fd; // Client File Descriptor
    int new_sock_fd; // new_socket File Descriptor for Client connection 
    struct sockaddr_in server_addr; // Server address
    struct sockaddr_in client_addr; // Client address
    int reuse; // Port and IP reusability parameter (0) - NIL , (1) - REUSE  

} TCP_param_t;

// for send and receive on a TCP socket 

/// @brief - Function to send/recv a single message and close the sockets (One Time use) 
/// @param - (TCP_param_t*) - Object of TCP_param_t struct 
/// @param - (void*) - Transmit buffer (NULL if not required)
/// @param - (void*) - Receive buffer (NULL if not required)
/// @return - STATUS_CHECK Enum (PASS/FAIL)
FUNC_CHECK TCP_send_recv(TCP_param_t* , void* , void* );


/// @brief - Function to continuosly be in poll mode and recv/send data when activity is found (can write to SHM)
/// @param - (TCP_param_t*) - Object of TCP_param_t struct 
/// @param - (void*) - Transmit buffer (NULL if not required)
/// @param - (void*) - Receive buffer (NULL if not required)
/// @param - (char*) - Name of the SHM (NULL if not required)
/// @return - STATUS_CHECK Enum (PASS/FAIL)
FUNC_CHECK TCP_send_recv_poll(TCP_param_t* , void* , void* , char* );
