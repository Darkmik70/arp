#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>



void create_pipes(int *pipe_set[])
{
    int num_pipes = sizeof(&pipe_set); // / sizeof(pipe_set)
    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(pipe_set[i]) < 0) /* Test for success */
        {
            printf("Unable to create a pipe; errno=%d\n", errno);
            exit(1); /* Print error message and exit */
        }
    }
    printf("Pipes created successfully\n");
}


int main()
{
    /* server initzialization*/
    int retval, m, n; 
    struct timeval tv;
    char line[80];
    int cell[2];

    /* Create unnamed pipes */
    int w0_ask[2], w0_ans[2], w1_ask[2], w1_ans[2], r0_ask[2], r0_ans[2], r1_ask[2], r1_ans[2];
    int *pipes[] = {w0_ask, w0_ans, w1_ask, w1_ans, r0_ask, r0_ans, r1_ask, r1_ans};

    fd_set readfds = {w0_ask[0], w0_ans[0], w1_ask[0], w1_ans[0], r0_ask[0], r0_ans[0], r1_ask[0], r1_ans[0]};
    
    create_pipes(pipes);

    
    while (true)
    {

        /* Wait up to five seconds. */ 
        tv.tv_sec = 5;  // It is a good practice to reainitialize timout each time select is invoked
        tv.tv_usec = 0;

        retval = select(1, &readfds, NULL, NULL, &tv);
        if (retval == -1)
            perror("select()");
        else if (retval)
        {

            printf("Data is available now.\n");
            // READ THE DATA
            /* FD_ISSET(0, &rfds) will be true. */
            n = read(0, line, 80);
            line[n] = '\0';
            m = write(1, line, n);


            if (cell[0] == cell[1] == 0)
            {
                // allow both W1 and W0
            }
            if (cell[0] <= cell[1]) 
            {
                // Accept W0
            }
            if (cell[1] <= cell[0])
            {
                // Accept W1
            }


        }
        else printf("No data within five seconds.\n");
    }



    // SERVER LOGIC
    /*
    accept one of the queries under condition
    conditions for W0, W1:
    - if cell[0]==cell[1]==0 accept both
    - if cell[0]<=cell[1] accept W0
    - if cell[1]<=cell[0] accept W1

    conditions for R0, R1
    - if cell[0] has changed accept R0
    - if cell[1] has changed accept R1
    */

    return 0;
}


/*
Hints:
    •you can read random numbers from /dev/random (see slides)
    •test separately the W and R processes
    •use ascii characters instead of integer
    •try different timeout values, including a zero value
    •count how many numbers have been stored in S by W0 and W1 and compare with the  
    logfiles lengths
    •try to estimate the speed (when timeout is zero in particular).
*/