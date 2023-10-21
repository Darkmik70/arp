#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/random.h>
#include <sys/select.h>

void write_message(int fd, char *msg);
void read_message(int fd, char *msg);

int main(int argc, char *argv[])
{


    int cell[2]; // Cell to be changed by W0 and W1
    int retval, m, n;
    char line[80];
    char msg_approve[80] = "1", msg_reject[80] = "0";
    int w0_ask, w0_ans /*, w1_ask[2], w1_ans[2], r0_ask[2], r0_ans[2], r1_ask[2], r1_ans[2]*/;
    int pipes[2] = {w0_ask, w0_ans /*, w1_ask, w1_ans, r0_ask, r0_ans, r1_ask, r1_ans*/};

    // This is not safe but it has to do for the time being
    char* format_string = "%d %d"; // maybe it will be better to pass arguments with a trailing coma 
    sscanf(argv[1], format_string, &w0_ask, &w0_ans);
    printf("New file descriptors created with given ids w0 = %d %d\n", w0_ask, w0_ans);

    fd_set readfds = {w0_ask /*, w0_ans[0]/*, wq1_ask[0], w1_ans[0], r0_ask[0], r0_ans[0], r1_ask[0], r1_ans[0]*/};
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(w0_ask, &readfds);

    while (true)
    {
        int tempCell[2];
        char msg[80];
        int bytesRead;

        /* Wait up to five seconds. */
        tv.tv_sec = 5; // It is a good practice to reainitialize timout each time select is invoked
        tv.tv_usec = 0;

        retval = select(w0_ask+1, &readfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            perror("select()");
        }
        else if (retval)
        {
            printf(" Pipe selected\n");
            /* Writer 0 */
            if (FD_ISSET(w0_ask, &readfds))
            {
                read_message(w0_ask, msg);
                if (msg[0] == 'R')
                {
                    printf("Writer 0 requests to WRITE");
                    // if (cell[0] <= cell[1])
                    if (true)
                    {
                        // Allow W0
                        /* Send message allowing w0 to write*/
                        printf("Writer 0 - request approved");
                        write_message(w0_ans, msg_approve);

                        /* await for the return value */
                        printf("Writer 0 - Waiting for the number");
                        read_message(w0_ask, msg);

                        printf(" THE MESSAGE FROM WRITER 0 is = %s", msg);
                        tempCell[0] = atoi(msg);
                    }
                    else
                    {
                        printf("Writer 0 - request rejected");
                    }
                }
            }
        }
        else
        {
            printf("No data within five seconds.\n");
        }
        cell[0] = tempCell[0];
        cell[1] = tempCell[1];
    }
    return 0;
}

/**
 * Writes a message to a file descriptor and checks if the write operation was successful.
 * @param fd: an integer representing the file descriptor to write to.
 * @param msg: a pointer to a character array representing the message to write.
 */
void write_message(int fd, char *msg)
{
    int retval;
    write(fd, msg, strlen(msg));
    if (retval != strlen(msg))
    {
        printf("Write did not return expected value\n");
        exit(1); /* Print error message and exit */
    }
    printf("Message sent successfully!");
}

/**
 * Reads a message from a file descriptor.
 * @param fd the file descriptor to read from
 * @param msg a pointer to a buffer to store the message
 */
void read_message(int fd, char *msg)
{
    int bytesRead;
    bytesRead = read(fd, msg, strlen(msg));
    if (bytesRead < 0)
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
    }
}