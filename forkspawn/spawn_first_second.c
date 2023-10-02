#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn(const char *program, char **arg_list)
{
  pid_t child_pid = fork();
  if (child_pid != 0)
    return child_pid;
  else
  {
    execvp(program, arg_list);
    perror("exec failed");
    return 1;
  }
}

int main()
{
  int first_pid, second_pid;
  char input_string[80]; // input to exit the file.
  int delay = 1;         // Delay between spawns in seconds

  // arguments for executables
  char *first_args[] = {"./first", NULL};
  char *second_args[] = {"./second", NULL};

  // spawn
  printf("Spawning first...\n");
  first_pid = spawn("./first", first_args);
  printf("Created child process %s with PID: %d\n",first_args[0], first_pid);
  sleep(delay);

  printf("Spawning second...\n");
  second_pid = spawn("./second", second_args);
  printf("Created child process %s with PID: %d\n",second_args[0], second_pid);
  sleep(delay);

  while (1)
  {
    /* read a full input line */
    fgets(input_string, 80, stdin);

    /* if the first input char is q, exit  */
    if (input_string[0] == 'q')
    {
      printf("Main program exiting...\n");
      // Killing the spawned processes
      printf("Killing first process with PID: %d\n", first_pid);
      kill(first_pid, SIGTERM); // or SIGKILL
      printf("Killing second process with PID: %d\n", second_pid);
      kill(second_pid, SIGTERM);

      exit(EXIT_SUCCESS);
    }
  }

  return 0;
}
