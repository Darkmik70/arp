#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int create_child(pid_t pid, char **arg_list);

void create_logfilename(char* logfile_name);


int main()
{
    // pids for all children
    pid_t watchdog;
    pid_t child_p0, child_p1, child_p2;

    // Create a logfilename with the startdate
    char logfile_name[80];
    create_logfilename(logfile_name);

    printf("This is logfilename: %s\n", logfile_name);
    
    int num_children = 0;
    // create watchdog

    // create child processes
    // Process 0
    char *p0_args[] =  {"konsole", "-e", "./simple_process", "0", NULL};
    create_child(child_p0, p0_args);
    num_children++;

    // Process 1
    char *p1_args[] =  {"konsole", "-e", "./simple_process", "1", NULL};
    create_child(child_p1, p1_args);
    num_children++;

    // Process 2
    char *p2_args[] =  {"konsole", "-e", "./simple_process", "2", NULL};
    create_child(child_p2, p2_args);
    num_children++;

    // wait for all children to die
    for (int i = 0; i < num_children; i++)
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
    printf("Exiting Master process.. \n");
    return 0;
}

//TODO add explanation
int create_child(pid_t pid, char **arg_list)
{
    pid = fork();
    if (pid < 0)
    {
        perror("Fork has failed");
        return -1;
    }
    else if (pid == 0)
    {
        execvp(arg_list[0], arg_list);
        return 0;
    }
    else 
    return -1;
}

//TODO add explanation
void create_logfilename(char* logfile_name)
{
    /* Get current date and time */
    time_t currentTime;
    time(&currentTime);
    // Convert to local time
    struct tm *localTime = localtime(&currentTime);
    // Format and print the time
    char formattedTime[80];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M:%S", localTime);
    
    sprintf(logfile_name, "./log/watchdog_%s.txt", formattedTime);

    // Check if log dir exists
    struct stat st = {0};
    if (stat("./log", &st)  == -1)
    {
        // Directory does not exist, we need to create a new one
        mkdir("./build/log", 0777);
    }
}
