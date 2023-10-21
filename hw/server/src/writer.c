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
    int fd_ask_write, fd_ans_read;  

    // This is not safe but it has to do for the time being
    char* format_string = "%d %d"; // maybe it will be better to pass arguments with a trailing coma 
    sscanf(argv[1], format_string, &fd_ask_write, &fd_ans_read);
    printf("New file descriptors created with given ids w0 = %d %d\n", fd_ask_write, fd_ans_read);

    while (1) 
    {
        char msg_ask[80] = "R", msg_ans[80]; 
        int ret_val;

        // Ask for the permission
        ret_val = write(fd_ask_write, msg_ask, strlen(msg_ask)); 
        if (ret_val != strlen(msg_ask))
        {
            printf("Write did not return expected value\n");
            exit(2);    /* Print error message and exit */
        }
        else
        {
            printf("Message sent successfully!\n");
        }
        // Wait for the answer
        ret_val = read(fd_ans_read,msg_ans, strlen(msg_ans));
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
                write(fd_ask_write, msg, strlen(msg));
            }
        } 
    }
    return 0; 
}