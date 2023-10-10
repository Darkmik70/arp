#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main(int argc, char *argv[])
{ 
    int fd; // file descriptor
    char * fifo = "/tmp/fifo"; // fifo is read only, awaits for the message from /first.c
    ssize_t bytesRead; // This var is used to check if anything arrives from fifo

    char str1[80];
    char format_string[80] = "%d %d,%d";
    int id, dest; // message specifies who processes the data
    int n1, n2;
    float mean;

    // identify itself - argc is the count of the arguments, and id is the last passed argument.
    id = atoi(argv[argc - 1]);
    printf("This is my id: %d\n", id);

    while (1) 
    { 
        fd = open(fifo, O_RDONLY); 
        bytesRead = read(fd, str1, 80); 

        if (bytesRead < 0 )
        {
            exit(EXIT_FAILURE);
        } 
        else if (bytesRead == 0) 
        {
            printf("No message received.\n");
            exit(EXIT_SUCCESS);
        } 
        else
        {
            printf("Message received!\n");

            /* read numbers from input line */
            sscanf(str1, format_string, &dest, &n1, &n2);

            if (str1[0] == 'q')
            {
                printf(" Bye \n");
                exit(EXIT_SUCCESS); 
            }
            else if (id == dest)
            {   
                mean = (n1 +n2) / 2.0;
                printf("mean value is: %f, sum is: %d \n", mean, n1 + n2); 
            }
            else 
            {
                printf("This message is not for me \n");
            }
        }
        close(fd);
    } 
    return 0; 
} 
