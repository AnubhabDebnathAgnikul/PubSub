// Common - Process

// Inlcudes 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// User Enum
typedef enum 
{
    FAILURE = -1,
    SUCCESS = 1,
} SHM_CHECK;

// Size of shared memory in bytes

/// @brief - Function to write value to SHM and replace old value in SHM with new 
/// @param - char * - Name of the SHM  
/// @param - size_t - Size of the SHM  
/// @param - char * - mapping pointer 
/// @param - char * - global write buffer   
/// @return - (Enum) SHM_CHECK FAILURE/SUCCESS 
SHM_CHECK SHM_init_write(char*, size_t, char*, char*);

/// @brief - Function to read a value from SHM 
/// @param - char * - Name of the SHM  
/// @param - size_t - Size of the SHM  
/// @param - void * - mapping pointer 
/// @param - char * - global read buffer 
/// @param - int - File descriptor of SHM 
/// @return - (Enum) SHM_CHECK FAILURE/SUCCESS 
SHM_CHECK SHM_read_close(char*, size_t, void* ,char *);


/// @brief - Delete and unlink from the SHM 
/// @param - Name of the SHM to be deleted (Unlinked)
/// @return - (Enum) SHM_CHECK FAILURE/SUCCESS  
SHM_CHECK SHM_del(char* );