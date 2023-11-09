#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MSG_MAX_LEN 80

int main(int argc, char *argv[])
{
    // sleep(20);
    int fd_read, fd_write; // file descriptors

    /*Assign correct fd numbers*/
    char *format_string = "%d %d"; // maybe it will be better to pass arguments with a trailing coma
    sscanf(argv[argc - 1], format_string, &fd_read, &fd_write);

    bool isReady = true; // ready during the first state

    char msg_send[MSG_MAX_LEN], msg_read[MSG_MAX_LEN];
    char msg_ask[80];
    msg_ask[0] = 'A';

    while (1)
    {
        int ret_val;
        if (isReady)
        {
            /* Ask for the permission to write*/
            printf("Asking for permission to write on fd: %d \n", fd_write);
            ret_val = write(fd_write, msg_ask, sizeof(msg_ask));
            printf("ret_Val = %d \n", ret_val);
            if (ret_val != sizeof(msg_ask))
            {
                printf("Write did not return expected value\n");
                sleep(5);
                exit(1); /* Print error message and exit */
            }
            isReady = false;
        }
        else
        {
            /* Wait for reply*/
            ret_val = read(fd_read, msg_read, sizeof(msg_read));
            if (ret_val != sizeof(msg_read))
            {
                printf("Read did not return expected value\n");
                sleep(5);
                exit(1); /* Print error message and exit */
            }
            /* Message is acquired*/
            if (msg_read[0] == 'Y')
            {
                printf("Request approved! generating number... \n");

                /* Generate message */
                // Initialize random seed
                srand(time(0));
                // Generate random integer between 0 and 255
                int random_num = rand() % 255;
                snprintf(msg_send,sizeof(msg_send), "%d", random_num);
                printf("Message generated: %s\n", msg_send);

                ret_val = write(fd_write, msg_send, sizeof(msg_send));
                if (ret_val != sizeof(msg_send))
                {
                    printf("Write did not return expected value\n");
                    sleep(5);
                    exit(1); /* Print error message and exit */
                }
                printf("Message %s sent succesfully to fd:%d\n", msg_send, fd_write);
            }
            else if (msg_read[0] == 'N')
            {
                printf("Request rejected\n");
            }
            else
            {
                /*Ideally this should never be reached*/
                printf("The message format is incorrect, message = {%s}\n", msg_read);
            }
            /*After reading the message allow writer to ask again*/
            isReady = true;
        }
        /* Wait at the end of the loop */
        sleep(2);
    }
    return 0;
}