#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/random.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    // sleep(20);
    int fd_ask[2], fd_ans[2];

    char *format_string = "%d %d";
    sscanf(argv[argc - 1], format_string, &fd_ask[0], &fd_ans[1]);

    fd_set rfds;

    char msg_no[80], msg_yes[80];
    msg_no[0] = 'N';
    msg_yes[0] = 'Y';

    while (1)
    {
        int temp_cell[2];
        struct timeval tv;
        /* Wait up to five seconds. */
        tv.tv_sec = 5; // It is a good practice to reainitialize timout each time select is invoked
        tv.tv_usec = 0;

        char msg_read[80];

        int retval, bytes_read;
        FD_ZERO(&rfds);
        FD_SET(fd_ask[0], &rfds);

        retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            perror("select()");
            sleep(5);
        }
        else if (retval)
        {
            printf("Pipe selected\n");
            for (int i = 0; i < FD_SETSIZE; i++)
            {
                if (FD_ISSET(i, &rfds))
                {
                    bytes_read = read(fd_ask[0], msg_read, sizeof(msg_read)); /* Read from pipe */
                    printf("This is the message %s  %d\n", msg_read, bytes_read);
                    if (bytes_read != sizeof(msg_read))
                    {
                        printf("Read did not return expected value\n");
                        exit(1); /* Print error message and exit */
                    }
                    if (msg_read[0] == 'A')
                    {
                        printf("Writer0 ask to write\n");
                        // logic
                        printf("Sending approve %s", msg_yes);
                        write(fd_ans[1], msg_yes, sizeof(msg_yes));
                    }
                    else
                    {
                        sscanf(msg_read, "%i", &temp_cell[0]);
                        printf("Writer 0 set cell 0 to %i \n", temp_cell[0]);
                    }
                }
            }
        }
        else
            printf("No data within %li seconds.\n", tv.tv_sec);

        sleep(2);
    }
}