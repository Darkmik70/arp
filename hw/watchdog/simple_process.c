#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include "include/constants.h"


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


int main(int argc, char *argv[]) 
{



} 