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
    int fd_ask[2], fd_ans[2]; // file descriptors

    pipe(fd_ask);
    pipe(fd_ans);

    /* Create child processes */
    pid_t child_writer_0, child_server;

    // Server
    char server_args[80];
    snprintf(server_args, sizeof(server_args), "%d %d", fd_ask[0], fd_ans[1]);
    char *server_args_list[] = {"konsole", "-e", "./server", server_args, NULL};
    child_server = create_child("konsole", server_args_list);


    char w0_args[80];
    snprintf(w0_args, sizeof(w0_args), "%d %d", fd_ask[1], fd_ans[0]);
    char *writer0_args_list[] = {"konsole", "-e", "./writer", w0_args, NULL};
    child_writer_0 = create_child("konsole", writer0_args_list);

    char r0_args[80];
    snprintf(w0_args, sizeof(w0_args), "%d %d", fd_ask[1], fd_ans[0]);
    char *writer0_args_list[] = {"konsole", "-e", "./writer", w0_args, NULL};
    child_writer_0 = create_child("konsole", writer0_args_list);




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
