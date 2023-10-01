#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>

static int counter;

static void body(const char *s, int delay)
{
    int i;

    for (i = 0; i < 6; i++) {
	sleep(delay);
        printf("%s: %d\n", s, counter++);
    }
}

int main(int argc, char *argv[])
{
    pid_t child;
   int res;

    counter = 0;

    child = fork();
    if (child < 0) {
        perror("Fork");

        return -1;
    }

    if (child == 0) {
        body("Child", 1);

	return 0;
    }
    body("Father", 2);

    wait(&res);

    return 0;
}
