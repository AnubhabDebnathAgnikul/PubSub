// defines config.h 

// Publisher side variables 
#define TOPIC_PUB "FLT" // Topic variable 
#define IP_ADDR_NEED_PUB "1.1.1.25" // IP_ADDR of subscriber that is requested by Publisher 
#define PORT_NEED_PUB "8030" // PORT of subscriber that is demanded by publisher 
#define OWN_IP "127.0.0.1" // IP of the Publisher device 
#define TCP_TX_MSG_BUFF "HELLO 8030" // character buffer as of now 
#define SYNC_PORT_PUB "8050" // Respective Internal Sync port that the Publisher connects to 

// Synchronizer 

//SYNC DEFINES - 1 - sync_proc_thread_1 variables 
#define SYNC_PORT_INT "8050" // PORT of the Internal TCP_Server()  of the SYNC_PROCESS
#define SYNC_PORT_EXT "8090" // PORT of the EXternal TCP_Server() of the SYNC_PROCESS
#define SHM_NAME_SYNC "/shared_mem_8050" // The SHM_name both the TCP_Servers write into *Note : Only one for each Device / Computer 

// SYNC DEFINES - 2 - TCP_lib variables 
#define MAX_SYNC_SERV 2 // No of SYNCHRONIZERS want to connect to 
static char *SYNC_CLIENT_IPS[MAX_SYNC_SERV] = {"127.0.0.1", "127.0.0.1"}; // Buffer of all the IP_ADDR of SYNCs in various devices 
static char *SYNC_CLIENT_PORTS[MAX_SYNC_SERV] = {"8091", "8092"}; // Buffer of all the PORT of SYNC in various deices 

// Change IPs and PORTs manually in C 

// SUBSCRIBER side variables 
#define SHM_NAME_SUB "/shared_mem_8050" // Name of the SHM the Subscriber process should read message from 
#define PORT_CHECK_SUB "8030" // Port check parameter (check for matching by tokenizing the Disc. Message)