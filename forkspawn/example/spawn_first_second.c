// analyze this code in details:
//A - what is expected to do as it is?
//B - what is expected to do when removing the "//" comments?
//C - are you able to use this method to execute "first"and "second" codes inside the previous exercise? Hint: use fork() to generate two processes, and let the two processes to execute sepratly two different executables
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


