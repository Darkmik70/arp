#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/random.h>
#include <sys/select.h>

#define fd_number 8 // TODO better naming

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Initialization, should only be called once. // needed for random number generation

    // sleep(20);
    //  w0_ask[0], w0_ans[1], r0_ask[0], r0_ans[1], w1_ask[0], w1_ans[1], r1_ask[0], r1_ans[1]);
    int w0_ask_read, w0_ans_write, r0_ask_read, r0_ans_write,
        w1_ask_read, w1_ans_write, r1_ask_read, r1_ans_write;

    char *format_string = "%d %d %d %d %d %d %d %d";
    sscanf(argv[argc - 1], format_string, &w0_ask_read, &w0_ans_write, &r0_ask_read, &r0_ans_write);

    fd_set rfds;

    char msg_no[80], msg_yes[80];
    msg_no[0] = 'N';
    msg_yes[0] = 'Y';

    while (1)
    {
        int cell[2], temp_cell[2];
        struct timeval tv;
        /* Wait up to five seconds. */
        tv.tv_sec = 5; // It is a good practice to reainitialize timout each time select is invoked
        tv.tv_usec = 0;

        char msg_read[80], msg_write[80];

        int retval, bytes_read;
        FD_ZERO(&rfds);
        FD_SET(w0_ask_read, &rfds);
        FD_SET(r0_ask_read, &rfds);
        FD_SET(w1_ask_read, &rfds);
        FD_SET(r1_ask_read, &rfds);

        retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            perror("select()");
            sleep(5);
        }
        else if (retval)
        {
            // TODO Generate random number generation for 3 : FD_SETSIZE
            int i = (rand() % (FD_SETSIZE - 3)) + 3; // Add + 1, then it allows for FD_SETSIZE
            printf("Pipe %d selected\n", i);

            switch (i)
            {
            case w0_ask_read:
                printf("w0 ask selected\n") break;
            case w1_ask_read:
            printf("w0 ask selected\n") case r0_ask_read:
            printf("w0 ask selected\n") case r1_ask_read:
                printf("w0 ask selected\n") break;
            case

                case
                /* code */
                break;

                default:
                break;
            }

            //     for (int i = 0; i < FD_SETSIZE; i++)  {
            //         if (FD_ISSET(i, &rfds) == w0_ask_read)
            //         {
            //             bytes_read = read(w0_ask_read, msg_read, sizeof(msg_read)); /* Read from pipe */
            //             printf("This is the message %s  %d\n", msg_read, bytes_read);
            //             if (bytes_read != sizeof(msg_read))
            //             {
            //                 printf("Read did not return expected value\n");
            //                 exit(1); /* Print error message and exit */
            //             }

            //             if (msg_read[0] == 'A')
            //             {
            //                 printf("Writer0 ask to write\n");
            //                 // logic
            //                 printf("Sending approve %s", msg_yes);
            //                 write(w0_ans_write, msg_yes, sizeof(msg_yes));
            //             }
            //             else
            //             {
            //                 sscanf(msg_read, "%i", &temp_cell[0]);
            //                 printf("Writer 0 set cell 0 to %i \n", temp_cell[0]);
            //             }
            //         }
            //         if (FD_ISSET(i, &rfds) == r0_ask_read)
            //         {
            //             bytes_read = read(r0_ask_read, msg_read, sizeof(msg_read)); /* Read from pipe */
            //             if (bytes_read != sizeof(msg_read))
            //             {
            //                 printf("Read did not return expected value\n");
            //                 exit(1); /* Print error message and exit */
            //             }
            //             printf("Reader 0 ask to write with message: %s \n", msg_read);

            //             // TODO check if value has changed
            //             if (true) // value has changed
            //             {
            //                 // writing Yes and sending what is the message
            //                 snprintf(msg_write, sizeof(msg_write), "%c %d", msg_yes[0], temp_cell[0]); // FIXME this is not temp_cell it is cell
            //             }
            //             else
            //             {
            //                 snprintf(msg_write, sizeof(msg_write), "%s", msg_no); // FIXME this is not temp_cell it is cell
            //             }
            //             printf("Writing into Reader 0: {%s}", msg_write);
            //             write(r0_ans_write, msg_write, sizeof(msg_write));
            //         }
            //     }
            // }
            // else printf("No data within %li seconds.\n", tv.tv_sec);
            sleep(2);
        }
    }

    // if cell[0]==cell[1]==0 accept both
    // if cell[0]<=cell[1] accept W0
    // // if cell[1]<=cell[0] accept W1
    // conditions for R0, R1
    // if cell[0] has changed accept R0
    // if cell[1] has changed accept R1