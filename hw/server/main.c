#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

int create_child(const char *program, char **arg_list);

int main()
{
    // file descriptors
    int w0_ask[2], w0_ans[2], w1_ask[2], w1_ans[2], r0_ask[2], r0_ans[2], r1_ask[2], r1_ans[2];

    pipe(w0_ask); pipe(w0_ans); pipe(r0_ask); pipe(r0_ans);
    pipe(w1_ask); pipe(w1_ans); pipe(r1_ask); pipe(r1_ans);


    /* Create child processes */
    pid_t child_writer_0, child_server;
    pid_t child_reader_0;
    // Server
    char server_args[80];
    snprintf(server_args, sizeof(server_args), "%d %d %d %d %d %d %d %d", //  $1 = "3 6 7 10 11 14 15 18"
     w0_ask[0], w0_ans[1], r0_ask[0], r0_ans[1], w1_ask[0], w1_ans[1], r1_ask[0], r1_ans[1]);
    char *server_args_list[] = {"konsole", "-e", "./server", server_args, NULL};
    child_server = create_child("konsole", server_args_list);


    char w0_args[80];
    snprintf(w0_args, sizeof(w0_args), "%d %d", w0_ask[0], w0_ans[1]);
    char *writer0_args_list[] = {"konsole", "-e", "./writer", w0_args, NULL};
    child_writer_0 = create_child("konsole", writer0_args_list);

    char r0_args[80];
    snprintf(r0_args, sizeof(r0_args), "%d %d", r0_ask[0], r0_ans[1]);
    char *reader0_args_list[] = {"konsole", "-e", "./reader", r0_args, NULL};
    child_reader_0 = create_child("konsole", reader0_args_list);





    bool run = true;
    while(run)
    {
    }
    return 0;


}


int create_child(const char *program, char **arg_list) // FIXME
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
