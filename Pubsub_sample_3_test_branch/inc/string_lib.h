#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/// @brief - Get all the sub string values from string tokenized by "/"
/// @param - char* - Buffer to be tokenized
/// @param - char* - Buffer to store Port_str (index - 2)
/// @param - char* - Buffer to store IP_Addr_str (index - 3)
/// @param - char* - Buffer to store status_str (index - 4) 
/// @return - (int) function status 
int str_tokenize(char* , char*, char* , char* );

/// @brief - Function accepts a string and replace the already existing character eith desired character  
/// @param - char * - Original main string 
/// @param - char * - Required substring found already in the main string 
/// @param - char * - Substring to be replaced old substr with 
/// @return - (void) NIL 
void updatesubstr(char* , const char* , const char* );