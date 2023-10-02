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
    char * fifo_one = "/tmp/fifo_one"; 
    char * fifo_two = "/tmp/fifo_two"; 
    int ready = 1; 

    // Create fifos for communication between first and second
    mkfifo(fifo_one, 0666); 
    mkfifo(fifo_two, 0666); 
  
    char input_string[80], send_string[80]; 

    while (1) 
    { 
        if (ready == 1)
        {
            
            fd1 = open(fifo_one, O_WRONLY); 
            printf("Please, write two integer numbers, separated by commas (,), or q to quit\n");
            
            /* to be sure that the previous is executed immediately */
            fflush(stdout);
            
            /* read a full input line */
            fgets(input_string, 80 , stdin); 

            // wait 4 seconds 
            sleep(4);

            write(fd1, input_string, strlen(input_string)+1); 
            close(fd1); 
            /* if the first input char is q, exit  */
            if (input_string[0] == 'q') exit(EXIT_SUCCESS) ;
                
            // set ready to 0
            ready = 0;
        }
        else
        {
            char msg[80];
            // Wait for message
            fd2 = open(fifo_two, O_RDONLY); 
            read(fd2, msg, 80); 
            printf("MESSAGE RECEIVED: %s\n", msg);
            close(fd2);
            ready = 1;
        }
        
    } 
    return 0; 
} 
