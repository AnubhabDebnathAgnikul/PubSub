// header file for TCP_new implementation

// Includes 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>

// Private Enum (or) structs
// Enum for checking 
typedef enum
{
    TCP_FAIL = -1, 
    TCP_PASS = 1,  
} TCP_CHECK;

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

// Base functions 
// 1. TCP_server()
// 2. TCP_client()

/// @brief - Function to create a TCP_client listens in specific PORT and for ALL_IPs
/// @param - (TCP_param_t*) - Object of TCP_param_t struct
/// @param - (char*) - PORT to listen in
/// @param - (int) - Maximum clients 
/// @return - STATUS_CHECK Enum (PASS/FAIL)
TCP_CHECK TCP_server(TCP_param_t* , char* , int );

/// @brief - Function to create a TCP_client to connect to server in specific PORT and IP_ADDR
/// @param - (TCP_param_t*) - Object of TCP_param_t struct
/// @param - (char*) - PORT to connect to 
/// @param - (char*) - IP_ADDR to connect to
/// @return - TCP_CHECK Enum (PASS/FAIL) 
TCP_CHECK TCP_client(TCP_param_t* , char* , char* );

/***********************************************************************************************************************/
// Custom send and receive functions 
// 1. TCP_send_recv()
// 2. TCP_send_recv_cont()
// 3. TCP_send_recv_poll()
// 4. TCP_send_recv_update()
/***********************************************************************************************************************/

// for send and receive on a TCP socket 

/// @brief - Function to send/recv a single message and close the sockets (One Time use) 
/// @param - (TCP_param_t*) - Object of TCP_param_t struct 
/// @param - (void*) - Transmit buffer (NULL if not required)
/// @param - (void*) - Receive buffer (NULL if not required)
/// @return - STATUS_CHECK Enum (PASS/FAIL)
TCP_CHECK TCP_send_recv(TCP_param_t* , void* , void* );

/// @brief - Function to continuosly send or receive message signal  
/// @param - (TCP_param_t*) - Object of TCP_param_t struct 
/// @param - (void*) - Transmit buffer (NULL if not required)
/// @param - (void*) - Receive buffer (NULL if not required)  
/// @return - STATUS_CHECK Enum (PASS/FAIL)
TCP_CHECK TCP_send_recv_cont(TCP_param_t* , void* , void* );

/// @brief - Function to continuosly be in poll mode and recv/send data when activity is found (can write to SHM)
/// @param - (TCP_param_t*) - Object of TCP_param_t struct 
/// @param - (void*) - Transmit buffer (NULL if not required)
/// @param - (void*) - Receive buffer (NULL if not required)
/// @param - (char*) - Name of the SHM (NULL if not required)
/// @return - STATUS_CHECK Enum (PASS/FAIL)
TCP_CHECK TCP_send_recv_poll(TCP_param_t* , void* , void* , char* );

/// @brief - Function to continuosly be in poll mode and recv/send data when activity is found (can write to SHM) (Use for internal synchronizer)
/// @param - (TCP_param_t*) - Object of TCP_param_t struct 
/// @param - (void*) - Transmit buffer (NULL if not required)
/// @param - (void*) - Receive buffer (NULL if not required)
/// @param - (char*) - Name of the SHM (NULL if not required)
/// @return - STATUS_CHECK Enum (PASS/FAIL)
TCP_CHECK TCP_send_recv_update(TCP_param_t* , void* , void* , char* );







