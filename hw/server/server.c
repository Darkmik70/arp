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

            sleep(2);
        }
    }

    // if cell[0]==cell[1]==0 accept both
    // if cell[0]<=cell[1] accept W0
    // // if cell[1]<=cell[0] accept W1
    // conditions for R0, R1
    // if cell[0] has changed accept R0
    // if cell[1] has changed accept R1