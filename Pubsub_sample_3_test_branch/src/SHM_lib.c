// Common - Process (Pub/Sub)

// SHM - WRITE Code
#include "SHM_lib.h"
#include <errno.h>
#include "string_lib.h"

// Global sample buffer
// char *g_rx_buff = 0;

// // Original implementation
// // Only appending to SHM
// SHM_CHECK SHM_init_write(char* shm_name , size_t shm_size, char* map_ptr, char* g_write_buff)
// {
//     // Open / Create SHM in Read/Write Mode
//     int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
//     if (shm_fd == -1)
//     {
//         perror("Sender: shm_open");
//         return FAILURE;
//     }

//     // Configure the size of the shared memory object (ftruncate)
//     if (ftruncate(shm_fd, shm_size) == -1)
//     {
//         perror("Sender: ftruncate");
//         return FAILURE;
//     }

//     // Memory mapping
//     map_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
//     if (map_ptr == MAP_FAILED)
//     {
//         perror("Sender: mmap");
//         return FAILURE;
//     }

//     // Write a message to the shared memory
//     // Use "memcpy" incase of a structure / "strcpy" incase of a string

//     // strcpy(map_ptr , g_write_buff); // Copying to the shm
//     memcpy(map_ptr, g_write_buff, strlen(g_write_buff) + 1);

//     // printf("Sender: Message written to shared memory.\n");

//     printf("[-] SHM : Msg written in SHM is: %s \n",g_write_buff);

//     // Unmapping from add. space
//     if (munmap(map_ptr, shm_size) == -1)
//     {
//         perror("Sender: munmap");
//         return FAILURE;
//     }

//     // Closing the shm
//     if (close(shm_fd) == -1)
//     {
//         perror("Sender: close");
//         return FAILURE;
//     }

//     // Remove the shared memory object
//     // if (shm_unlink(SHARED_MEM_NAME) == -1)
//     // {
//     //     perror("Sender: shm_unlink");
//     //     return FAIL;
//     // }

//     // printf("Sender: Shared memory removed.\n");

//     return SUCCESS;
// }

// // Updation into the shared memory version
SHM_CHECK SHM_init_write(char *shm_name, size_t shm_size, char *map_ptr, char *g_write_buff)
{
    char status_buff[5];
    char check_buff[50];
    char actual_buff[50];

    // Open / Create SHM in Read/Write Mode
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("Sender: shm_open");
        return FAILURE;
    }

    // Configure the size of the shared memory object (ftruncate)
    if (ftruncate(shm_fd, shm_size) == -1)
    {
        perror("Sender: ftruncate");
        return FAILURE;
    }

    // Memory mapping
    map_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (map_ptr == MAP_FAILED)
    {
        perror("Sender: mmap");
        return FAILURE;
    }

    printf("[-] SHM : VALUE ALREADY IN SHM: %s \n", map_ptr);

    // Checking mechanism. 
    
    memcpy(check_buff, g_write_buff, strlen(g_write_buff) + 1);

    memcpy(actual_buff, g_write_buff, strlen(g_write_buff) + 1);

    str_tokenize(g_write_buff, NULL, NULL, status_buff);

    if (strcmp(status_buff, "1") == 0)
    {
        updatesubstr(check_buff, "1", "0");
    }

    else if (strcmp(status_buff, "0") == 0)
    {
        updatesubstr(check_buff, "0", "1");
    }

    for (int i = 0; i < shm_size / sizeof(actual_buff); i++)
    {

        if (strcmp(map_ptr, check_buff) == 0)
        {
            strncpy(map_ptr, actual_buff, strlen(actual_buff) + 1);
        }

        else if ((map_ptr != check_buff))
        {
            strncpy(map_ptr, actual_buff, strlen(actual_buff) + 1);
        }

    }

    printf("[-] SHM : VALUE OVER-WRITTEN INTO SHM '%s' = %s \n", shm_name, map_ptr);

    memset(check_buff, 0, strlen(check_buff) + 1);

    memset(actual_buff, 0, strlen(actual_buff) + 1);

    // Unmapping from add. space
    if (munmap(map_ptr, sizeof(g_write_buff)) == -1)
    {
        perror("Sender: munmap");
        return FAILURE;
    }

    // Closing the shm
    if (close(shm_fd) == -1)
    {
        perror("Sender: close");
        return FAILURE;
    }

    // // * Dont Include this *
    // // Remove the shared memory object
    // if (shm_unlink(shm_name) == -1)
    // {
    //     perror("Sender: shm_unlink");
    //     return FAILURE;
    // }

    // printf("Sender: Shared memory removed.\n");

    return SUCCESS;

}

SHM_CHECK SHM_read_close(char *shm_name, size_t shm_size, void *map_ptr, char *g_read_buff)
{
    
    // char rx_buff[] = {0};

    // Open the shared memory object
    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1)
    {
        perror("Receiver: shm_open");
        return FAILURE;
    }

    // Memory Mapping
    map_ptr = mmap(NULL, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (map_ptr == MAP_FAILED)
    {
        perror("Receiver: mmap");
        return FAILURE;
    }

    // Read the message from the shared memory
    // Use "memcpy" for structure / "strcpy" to copy single string
    // printf("Read: Message received: %s\n", (char*) map_ptr);

    // strcpy(g_read_buff, map_ptr);

    while (map_ptr != NULL)
    {
        memcpy(g_read_buff, map_ptr, strlen(map_ptr) + 1);
        printf("[-] SHM: map_ptr value is found \n");
        break;
    }

    // printf("[-] SHM : Msg read from SHM is: %s \n",g_read_buff);

    // Unmapping
    if (munmap(map_ptr, shm_size) == -1)
    {
        perror("Receiver: munmap");
        return FAILURE;
    }

    // Closing
    if (close(shm_fd) == -1)
    {
        perror("Receiver: close");
        return FAILURE;
    }

    // // Unlinking
    // if (shm_unlink(shm_name) == -1)
    // {
    //     perror("shm_unlink");
    //     return FAILURE;
    // }

    return SUCCESS;
}

// Unlinking function.
SHM_CHECK SHM_del(char *SHM_name)
{
    // Unlinking
    if (shm_unlink(SHM_name) == -1)
    {
        perror("shm_unlink");
        return FAILURE;
    }

    printf("[-] SHM : Deleting the contents of SHM - '%s'\n", SHM_name);
    return SUCCESS;
}
