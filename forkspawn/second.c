
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
  
int main() 
{ 
    int fd1, fd2; 
  
    char * fifo_one = "/tmp/fifo_one"; // fifo_one is read only, awaits for the message from /first.c
    char * fifo_two = "/tmp/fifo_two"; // fifo_two is write only, sends the message to /first.c

    char str1[80], str2[80]; 
    char format_string[80]="%d,%d";
    int n1, n2;
    double mean;
    char msg[80]; 

    // This var is used to check if anything arrives from fifo
    ssize_t bytesRead; 

    while (1) 
    { 
    
        fd1 = open(fifo_one,O_RDONLY); 
        
        bytesRead = read(fd1, str1, 80); 

        if (bytesRead < 0 )
        {
            exit(EXIT_FAILURE);
        } 
        else if (bytesRead == 0) 
        {
            printf("No message received./n");
            exit(EXIT_SUCCESS);
        }    
        else
        {
            /* if the first input char is q, exit  */
            if (str1[0] == 'q') exit(EXIT_SUCCESS) ;
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
       
        close(fd1);
    } 
    return 0; 
} 
