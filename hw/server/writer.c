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
    /* Get file descriptors*/    
    int fd_ask[2], fd_ans[2];  


    while (1) 
    {
        char msg_ask[80] = "READY", msg_ans[80]; 
        int ret_val;

        // Ask for the permission
        ret_val = write(fd_ask[1], msg_ask, strlen(msg_ask)); 
        if (ret_val != strlen(msg_ask))
        {
            printf("Write did not return expected value\n");
            exit(2);    /* Print error message and exit */
        }
        // Wait for the answer
        ret_val = read(fd_ans[0],msg_ans, strlen(msg_ans));
        if (ret_val != strlen(msg_ans)) 
        {
            printf("Read did not return expected value\n");
            exit(3);    /* Print error message and exit */
        }
        else
        {
            // Message acquired
            if (atoi(&msg_ans[0]) == 0)
            {
                // Request Rejected
                int time = 4;
                sleep(time);
                continue;
            }
            else
            {
                // Request Approved
                 int val;
                getrandom(&val, sizeof(val), 0); // RNG
                /* Put generated value into the message */
                char msg[80]; //size of ascii is 20
                snprintf(msg, strlen(msg), "%d", val);
                /* Write the message into first pipeline*/
                write(fd_ask[1], msg, strlen(msg));
            }
        } 
    }
    return 0; 
}