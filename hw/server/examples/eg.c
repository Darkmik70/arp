#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
int main(void)
{
    fd_set rfds;
    struct timeval tv;
    int retval, m, n;
    char line[80];
    /* Loop forever */
    while (1)
    {
        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        /* Wait up to five seconds. */
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        retval = select(1, &rfds, NULL, NULL, &tv);
        /* Don’t rely on the value of tv now! */
        if (retval == -1)
            perror("select()");
        else if (retval)
        {
            printf("Data is available now.\n");
            /* FD_ISSET(0, &rfds) will be true. */
            n = read(0, line, 80);
            line[n] = '\0';
            m = write(1, line, n);
        }
        else
            printf("No data within five seconds.\n");
    }
}