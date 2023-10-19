#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void create_pipes(int *pipe_set[]);
int summon(const char *program, char **arg_list); // FIXME

int main()
{
    pid_t child_server, child_w0, child_w1, child_r0, child_r1;

    int w0_ask[2], w0_ans[2], w1_ask[2], w1_ans[2];
    int r0_ask[2], r0_ans[2], r1_ask[2], r1_ans[2];

    /* Create unnamed pipes */
    int *pipes[] = {w0_ask, w0_ans, w1_ask, w1_ans, r0_ask, r0_ans, r1_ask, r1_ans};
    create_pipes(pipes);

    /* Create child processes*/
    char server_args[80], w0_args[80], reader_args[80];

    snprintf(w0_args, 80, "%d %d", w0_ask[0], w0_ans[1]);

    char *arg_list[] = {"konsole", "-e", "./server", w0_args, NULL};

    printf("Creating server...\n");
    child_server = summon("konsole", arg_list);

    // printf("Creating Writer_0....\n");
    // child_w0 = summon("konsole", w0_arg_list);

    return 0;
}




/**
 * Creates pipes for inter-process communication.
 *
 * @param pipe_set an array of integer arrays to store the file descriptors of the pipes
 */
void create_pipes(int *pipe_set[])
{
    int num_pipes = sizeof(&pipe_set) / sizeof(int *);

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

int summon(const char *program, char **arg_list) // FIXME
{
    pid_t child_pid = fork();
    if (child_pid != 0)
    {
        printf("Created child process %s with PID: %d\n", program, child_pid);
        return child_pid;
    }
    else
    {
        execvp(program, arg_list);
        perror("exec failed");
        return 1;
    }
}
