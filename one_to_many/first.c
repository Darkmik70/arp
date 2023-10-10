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
    char input_string[80], send_string[80];

    char * fifo = "/tmp/fifo"; 
    mkfifo(fifo, 0666); 

    while (1) 
    {

        fd = open(fifo, O_WRONLY);
        printf("Please, select which 'second' will process the data,"
               " then write two integer numbers, separated by commas (,), or q to quit\n"
               " eg: 1 2,3 \n");

        /* to be sure that the previous is executed immediately */
        fflush(stdout);

        /* read a full input line */
        fgets(input_string, 80, stdin);

        // wait 2 seconds
        sleep(2);

        write(fd, input_string, strlen(input_string) + 1);
        close(fd);

        /* if the first input char is q, exit  */
        if (input_string[0] == 'q')
            exit(EXIT_SUCCESS);
    } 
    return 0; 
} 
