// Unit testing code ( main() ) for all functions in SHM_lib.c
// Functions tested
// 1. SHM_init_write()
// 2. SHM_read_close() 
// 3. SHM_del()

// Private includes 
#include "SHM_lib.h"
#include "string_lib.h"

int main()
{
    char* shm_name_test = "/sample_SHM_test";
    size_t shm_size_test = 1000;
    int SHM_FD;

    // for SHM_init_write()
    char* map_ptr_test;
    char write_buffer_test[] = "FLT/1.1.1.40/8030/127.0.0.1/0"; 
    char dummy_buff[50];

    memcpy(dummy_buff, write_buffer_test, sizeof(write_buffer_test));

    // for SHM_read_close()
    char read_buffer_test[50];
    void* map_ptr_test_1;

    // 1.
    SHM_init_write(shm_name_test, shm_size_test, map_ptr_test, dummy_buff);

    usleep(500);

    // 2.
    SHM_read_close(shm_name_test, shm_size_test, map_ptr_test_1, read_buffer_test);

    if (strcmp(write_buffer_test, read_buffer_test) == 0)
    {
        printf("SUCCESS - VALUE MATCH\n");
    }
    else 
    {
        printf("FAIL - VALUE MISMATCH\n");
    }

    // 3. 
    SHM_close(SHM_FD);

    return 0;

}