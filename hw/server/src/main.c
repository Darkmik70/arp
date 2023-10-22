#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void create_pipes(int *pipe_set[], int num_pipes);
int summon(const char *program, char **arg_list); // FIXME

int main()
{
    pid_t child_server, child_w0, child_w1, child_r0, child_r1;

    int w0_ask[2], w0_ans[2], w1_ask[2], w1_ans[2];
    int r0_ask[2], r0_ans[2], r1_ask[2], r1_ans[2];

    /* Create unnamed pipes */
    int *pipes[] = {w0_ask, w0_ans, w1_ask, w1_ans, r0_ask, r0_ans, r1_ask, r1_ans};
    int num_pipes = sizeof(pipes) / sizeof(pipes[0]);
    pipe(w0_ask);
    pipe(w0_ans);
    pipe(w1_ask);
    pipe(w1_ans);
    pipe(r0_ask);
    pipe(r0_ans);
    pipe(r1_ask);
    pipe(r1_ans);
    // create_pipes(pipes, num_pipes);

    char w0_args[80], w1_args[80], r0_args[80], r1_args[80];
    snprintf(w0_args, sizeof(w0_args), "%d %d %d %d", w0_ask[0], w0_ask[1], w0_ans[0], w0_ans[1]);
    snprintf(w1_args, sizeof(w1_args), "%d %d %d %d", w1_ask[0], w1_ask[1], w1_ans[0], w1_ans[1]);
    snprintf(r0_args, sizeof(r0_args), "%d %d %d %d", r0_ask[0], r0_ask[1], r0_ans[0], r0_ans[1]);
    snprintf(r1_args, sizeof(r1_args), "%d %d %d %d", r1_ask[0], r1_ask[1], r1_ans[0], r1_ans[1]);

    /* Create child processes*/
    printf("Creating server...\n");
    char *server_arg_list[] = {"konsole", "-e", "./server", w0_args, w1_args, r0_args, r1_args, NULL};
    child_server = summon("konsole", server_arg_list);

    printf("Creating Writer_0....\n");
    char *writer0_args_list[] = {"konsole", "-e", "./writer", w0_args, NULL};
    child_w0 = summon("konsole", writer0_args_list);

    wait();
    return 0;
}

/**
 * Creates pipes for inter-process communication.
 *
 * @param pipe_set an array of integer arrays to store the file descriptors of the pipes
 */
void create_pipes(int *pipe_set[], int num_pipes)
{
    for (int i = 0; i < num_pipes; i++)
    {

        if (pipe(pipe_set[i]) < 0) /* Test for success */
        {
            printf("Unable to create a pipe; errno=%d\n", errno);
            exit(1); /* Print error message and exit */
        }
    }
    printf("%d Pipes created successfully\n", num_pipes);
}

int summon(const char *program, char **arg_list) // FIXME
{
    pid_t child_pid = fork();
    if (child_pid > 0)
    {
        printf("Created child process %s with PID: %d\n", program, child_pid);
        return child_pid;
    }
    else if (child_pid == 0)
    {
        execvp(program, arg_list);
        perror("exec failed");
        exit(1);  // Terminate the process if exec fails
    }
    else
    {
        perror("fork failed");
        return -1;  // Return an error code
    }
}
