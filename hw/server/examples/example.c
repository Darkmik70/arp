/*
This program must be run with two arguments:
1) a file name containing an ascii txt
2) an executable file that will read the previous and print

First the ascii file is opened returning a file descriptor
Then the executable is run passing to it as argument the file descriptor number.

Note that the executable shares the same file descriptor (stored in the u-area)

*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> /* for atoi() */

    int main(int argc,char *argv[]) {
	char str[15];
	int fd, fd1;
        if (argc != 3) {
            printf("Usage: %s  file to open file to run\n",argv[0]);
            return 1;
        }
	fd=open(argv[1], O_RDONLY);
	if (fd==-1)
		{perror ("error opening file");
		return 1;
		}
	sprintf(str, "%d", fd);
  	fd1=execl(argv[2], argv[2], str, NULL);

	if (fd1==-1)
		{perror ("error executing file");
		return 1;
		}
        return 0;
    }
