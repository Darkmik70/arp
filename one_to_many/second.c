
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
  
int main() 
{ 
    int fd; // file descriptor
  
    char * fifo = "/tmp/fifo"; // fifo is read only, awaits for the message from /first.c

    char str1[80], str2[80]; 
    char format_string[80]="%d,%d";
    int n1, n2;
    double mean;
    char msg[80]; 

    // This var is used to check if anything arrives from fifo
    ssize_t bytesRead; 



    while (1) 
    { 
        fd = open(fifo,O_RDONLY); 
        
        bytesRead = read(fd, str1, 80); 

        if (bytesRead < 0 )
        {
            exit(EXIT_FAILURE);
        } 
        else if (bytesRead = 0) 
        {
            printf("No message received./n");
            exit(EXIT_SUCCESS);
        }    
        else
        {
            /* if the first input char is q, exit  */
            if (str1[0] == 'q')
            {
                printf(" Bye \n");
                exit(EXIT_SUCCESS) ;
            }
            else if (str1[0] == '2')
            {
                
                sleep(2);
            }
            /* read numbers from input line */
            sscanf(str1, format_string, &n1, &n2);
            mean = (n1 + n2) / 2.0; 
            
            // Generate message
            snprintf(msg, sizeof(msg), "mean value is: %f, sum is: %d", mean, n1 + n2); 
            printf("%s\n", msg);
            // open second fifo, send the message.
            fd2 = open(fifo_two,O_WRONLY);
            write(fd2, msg, strlen(msg)+1);
            close(fd2);
        }
       
        close(fd);
    } 
    return 0; 
} 
