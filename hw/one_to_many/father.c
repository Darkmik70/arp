#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int create_child(const char *program, char **arg_list)
{
    pid_t child_pid = fork();
    if (child_pid != 0)
    {
        printf("Child process %s with PID: %d  was created\n", program, child_pid);
        return child_pid;
    }
    else
    {   
        //Fork failed
        execvp(program, arg_list);
        perror("exec failed");
        return 1;
    }
}

int main()
{
    pid_t first, sec1, sec2;

    // Arguments
    char* konsole = "konsole";
    char* first_args[] = {konsole, "-e", "./first", NULL}; 
    
    /* This is cumbersome | By having different id, 
        second will be able to tell when it has to process the message from the first */
    char* sec_args_1[] = {konsole, "-e", "./second", "1", NULL};
    char* sec_args_2[] = {konsole, "-e", "./second", "2", NULL};

    // create children
    first = create_child(konsole, first_args);
    sec1 = create_child(konsole, sec_args_1);
    sec2 = create_child(konsole, sec_args_2);

    // wait for all the children to die.
    for (int i = 0; i < 3; i++) // threshold 3 as we have 3 children
    {
        pid_t process;
        int child_exit_status;

        process = wait(&child_exit_status);

        if (WIFEXITED(child_exit_status))
        {
            printf("Child process with PID: %d has exited with status %d\n",
             process, WEXITSTATUS(child_exit_status));
        }
        else if (WIFSIGNALED(child_exit_status))
        {
            printf("Child process with PID: %d was killed by signal %d\n",
            process, WTERMSIG(child_exit_status));
        }
    } 
    printf("Exiting Father process.. \n");
    return 0;
}