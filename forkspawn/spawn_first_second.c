#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn(const char * program, char ** arg_list) {
  pid_t child_pid = fork();
  if (child_pid != 0)
    return child_pid;
  else {
    execvp (program, arg_list);
    perror("exec failed");
    return 1;
 }
}

int main() {
  int delay = 3; // Delay between spawns in seconds

  // arguments for executables
  char* first_args[] = {"./first", NULL};
  char* second_args[] = {"./second", NULL};

  // spawn
  printf("Spawning first...\n");
  spawn("./first", first_args);
  sleep(delay);
  
  printf("Spawning second...\n");
  spawn( "./second", second_args);
  sleep(delay);

  printf ("Main program exiting...\n");
  return 0;
}
