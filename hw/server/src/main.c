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

    int w0_ask_read = w0_ask[0];
    int w0_ask_write = w0_ask[1];
    int w0_ans_read = w0_ans[0];
    int w0_ans_write = w0_ans[1];

    int w1_ask_read = w1_ask[0];
    int w1_ask_write = w1_ask[1];
    int w1_ans_read = w1_ans[0];
    int w1_ans_write = w1_ans[1];

    int r0_ask_read = r0_ask[0];
    int r0_ask_write = r0_ask[1];
    int r0_ans_read = r0_ans[0];
    int r0_ans_write = r0_ans[1];

    int r1_ask_read = r1_ask[0];
    int r1_ask_write = r1_ask[1];
    int r1_ans_read = r1_ans[0];
    int r1_ans_write = r1_ans[1];

    /* Create child processes*/
    char server_args[80], writer_args[80], reader_args[80];

    // // Server requires only reading from requests and write answers
    // printf("Creating server...");
    // snprintf(server_args, 80, "%d %d", w0_ask_read, w0_ans_write);
    // char *server_arg_list[] = {"konsole", "-e", "./server", server_args, NULL};
    // child_server = summon("konsole", server_arg_list);

    // printf("Creating Writer_0....\n");
    // snprintf(writer_args, 80, "%d %d", w0_ask_write, w0_ans_read);
    // char *writer_args_list[] = {"konsole", "-e", "./writer", writer_args, NULL};
    // child_w0 = summon("konsole", writer_args_list);

    while (1)
    {
    }
    return 0;
}

/**
 * Creates pipes for inter-process communication.
 *
 * @param pipe_set an array of integer arrays to store the file descriptors of the pipes
 */
void create_pipes(int *pipe_set[])
{
    int num_pipes = sizeof(&pipe_set);

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
