#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

void log_message(FILE *fp, const char *message);

//  R0 asks reading cell 0 if its value has changed and writes into a logfile F0

int main(int argc, char *argv[])
{
    int sleep_time = 2;
    // Open for appending to keep the log
    FILE *log_file = fopen("logs/logfile.log", "a"); // TODO add index for
    int fd_write, fd_read;
    ssize_t bytesRead;

    /*Assign correct fd numbers*/
    char *format_string = "%d %d";
    sscanf(argv[argc - 1], format_string, &fd_write, &fd_read);

    bool isReady = true; // ready during the first state
    while (true)
    {
        char msg_ask[80], msg_send[80], msg_read[80];
        msg_ask[0] = '?';
        int ret_val;

        // Ask if value has changed
        if (isReady)
        {
            printf("Asking the server if the value has changed... \n");
            ret_val = write(fd_write, msg_ask, sizeof(msg_ask));
            if (ret_val != sizeof(msg_ask))
            {
                printf("Write did not return expected value\n");
                sleep(5);
                exit(1);
            }
            isReady = false;
        }
        else
        {
            printf("Waiting for response from the Server... \n");
            ret_val = read(fd_read, msg_read, sizeof(msg_read));
            if (ret_val != sizeof(msg_read))
            {
                printf("Read did not return expected value\n");
                sleep(5);
                exit(1); /* Print error message and exit */
            }
            printf("The response from the server is: %s \n", msg_read);

            if (msg_read[0] == 'Y')
            {
                // Read the data
                int data;
                char c[10];
                sscanf(msg_read, "%c %d", &c[0], &data);

                printf("Data has changed, writing into a log file ...\n");
            
                // TODO implement log file logic

                // Open the log file
                FILE *log_file = fopen("logfile.log", "a"); // Open for appending to keep the log

                // Log some messages
                char message[80];
                snprintf(message, sizeof(message), "The data has Changed... New data: %d", data);
                log_message(log_file, message);

                // Close the log file
                fclose(log_file);
                return 0;
            }
            else if (msg_read[0] == 'N')
            {
                printf("Data has not changed...\n");
            }
            else
            {
                /*Ideally this should never be reached*/
                printf("The message format is incorrect, message = {%s}\n", msg_read);
            }
            // Server answered, send new request
            isReady = true;
        }
        sleep(sleep_time);
    }
    return 0;
}

void log_message(FILE *fp, const char *message)
{
    if (fp == NULL)
    {
        perror("Error opening log file");
        exit(1);
    }
    fprintf(fp, " %s\n", message);
    // Flush the output to make sure it's written to the file
    fflush(fp);
}
