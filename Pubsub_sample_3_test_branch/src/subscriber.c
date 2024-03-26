// Subscriber process checking for the discovery message and then connecting to the respective server
// with assigned port and IP address 

// Private includes 
#include "subscriber.h"
#include "SHM_lib.h"
#include "TCP_lib.h" 
#include "string_lib.h"

TCP_param_t TCP_1;

int subscriber_proc(char* shm_name_1, char* TCP_recv_buff, char* check_port)
{    
    
   // Variable initializations

   // SHM read func. parameters (used by SHM_Read_Close() function)
   size_t shm_size_1 = 1000;
   void* mapping_ptr_1;
   char g_shm_read_buff[1000];

  // Token parameters (used by str_tokenize() function) 
   char status_buff[4];
   char port_buffer[10];
   char IP_addr_buffer[15];     
   
   // Super - Loop start
   while(1)
   {

      usleep(2000); // User delay for synchronization with publisher


      // Step - 1 Reading from specified shared memory 
      int SHM_check = SHM_read_close(shm_name_1, shm_size_1, mapping_ptr_1, g_shm_read_buff);

      // Step - 2[i] - If Value Not found in SHM_read_buff then go into rec loop 
      if ((SHM_check < 0) && (g_shm_read_buff == NULL))
      {
        printf("In recovery loop....\n");
      }

      // Step - 2[ii] - Else if any value is found in SHM_read_buff the proceed inside the else loop 
      else if((SHM_check > 0) && (g_shm_read_buff != NULL))
      {

        // Step - 3 - Tokenize the Read SHM_read_buffer and get port_need,IP_addr_need and status_bit   
        str_tokenize(g_shm_read_buff, port_buffer, IP_addr_buffer, status_buff);

        // Step - 4 Check if the Disc. message read from the SHM_read_buff satisfies all the conditions 
        // Step - 4[i] - If all conditions are satisfied follow through to Step - 5
        if (strcmp(status_buff, "1") == 0)
        {
            printf("status string is %s \n", status_buff);
        
            printf("Port is : %s\n", port_buffer);
        
            printf("IP is : %s\n", IP_addr_buffer);
          
            if (strcmp(port_buffer, check_port) == 0)  
            {   
                // Step - 5 - If all the conditions are satisfied then Form a TCP_server that listens to ANY_IPADDR and in port as PORT_NEED
                // *note : still Continue in the same While(1) loop  
                if (TCP_client(&TCP_1,port_buffer, "127.0.0.1") > 0) // Changed to client in Subscriber side 
                {
                  if (TCP_send_recv_cont(&TCP_1, NULL, TCP_recv_buff) > 0)
                  {
                    continue;
                  }
                }
            
            }

            // If Step - 4[ii] (does not satisfy) - then come here and do these steps 
            else
            {
              printf("Port not matching clearing memory \n");

              continue;

            }

        }

        else
        {
          printf("Status is not 1 clearing memory \n");

          continue;
        }

      }
      

      // Step - 6 (Optional Step) Clearing all the buffers and Unlinking from the SHM 
      // All the Temproary Buffers that are used to hold data temproarily gets memory cleared here 
      memset(g_shm_read_buff, 0, sizeof(g_shm_read_buff)); // SHM_read_buff
      memset(TCP_recv_buff, 0, sizeof(TCP_recv_buff)); // TCP_recv_buff
      memset(port_buffer, 0, sizeof(port_buffer));
      memset(IP_addr_buffer, 0, sizeof(IP_addr_buffer));
      memset(status_buff, 0, sizeof(status_buff));
      
      // Completely unlinking from the respective SHM
      SHM_del(shm_name_1);

    } // Super loop end 

    return 0;
}
   



   


