#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/random.h>
#include <unistd.h> 

  
int main(int argc,char *argv[])
{ 
    /*Decrypt fd names*/

    int fd_ask[2], fd_write[2];  


    char msg_request[80], msg_value[80]; 

    while (1) 
    { 
        // ASK FOR THE PERMISION



        /*RNG*/
        int val;
        getrandom(&val, sizeof(val), 0);
        /* read a full input line */
        fgets(msg, 80 , stdin); 
        write(fd[0], msg, strlen(msg)+1); 
  
    }
    return 0; 
}