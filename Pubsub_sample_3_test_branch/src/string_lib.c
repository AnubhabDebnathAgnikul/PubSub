#include "string_lib.h"

int str_tokenize(char* shm_recv_buff, char* port_str_buff, char* IP_str_buff, char* status_str_buff)
{

    // Token parameters 
   char delim[] = "/";
   char *token_1;

   char* substr_need[25];

   // Token pointers (Hard-coded)
   int TCP_port = 2;
   int TCP_IPaddr = 3;
   int status_str = 4;

   // counter 
   int current_count = 0;

   token_1 = strtok(shm_recv_buff, delim);

   while (token_1 != NULL)
    {
    
        substr_need[current_count++] = token_1;
        
        token_1 = strtok(NULL, delim);
   
    }

    if (status_str_buff != NULL)
    {
        memcpy(status_str_buff, substr_need[status_str], strlen(substr_need[status_str]) + 1);
    }

    if (port_str_buff != NULL)
    {
        memcpy(port_str_buff, substr_need[TCP_port], strlen(substr_need[TCP_port]) + 1);
    }

    if (IP_str_buff != NULL)
    {
        memcpy(IP_str_buff, substr_need[TCP_IPaddr], strlen(substr_need[TCP_IPaddr]) + 1);
    }

    return 0;


} 


void updatesubstr(char *main_str, const char *old_substr, const char *new_substr) 
{
    char *pos = main_str;
    char *last_pos = NULL;

    // Find the last occurrence of the old substring
    while ((pos = strstr(pos, old_substr)) != NULL) 
    {
        last_pos = pos;
        pos += strlen(old_substr);
    }

    if (last_pos != NULL) 
    {
        // Calculate the length of the prefix before the last occurrence
        size_t prefix_len = last_pos - main_str;

        // Calculate the length of the suffix after the last occurrence
        size_t suffix_len = strlen(last_pos + strlen(old_substr));

        // Create a new string with the updated substring
        char updated_str[strlen(main_str) - strlen(old_substr) + strlen(new_substr) + 1];
        strncpy(updated_str, main_str, prefix_len);
        updated_str[prefix_len] = '\0';  // Null-terminate the prefix
        strcat(updated_str, new_substr);
        strcat(updated_str, last_pos + strlen(old_substr));  // Add the suffix

        // Update the main string with the new value
        strcpy(main_str, updated_str);
    }
}