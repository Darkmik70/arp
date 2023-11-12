#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/random.h>
#include <sys/select.h>
#include <time.h>

#define fd_number 8 // TODO better naming

int main(int argc, char *argv[])
{
    /* Pipes */
     //  w0_ask[0], w0_ans[1], r0_ask[0], r0_ans[1], w1_ask[0], w1_ans[1], r1_ask[0], r1_ans[1]);
    int w0_ask_read, w0_ans_write, r0_ask_read, r0_ans_write,
        w1_ask_read, w1_ans_write, r1_ask_read, r1_ans_write;

    char *format_string = "%d %d %d %d %d %d %d %d";
    sscanf(argv[argc - 1], format_string, &w0_ask_read, &w0_ans_write, &r0_ask_read, &r0_ans_write,
           &w1_ask_read, &w1_ans_write, &r1_ask_read, &r1_ans_write);

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(w0_ask_read, &rfds);
    FD_SET(r0_ask_read, &rfds);
    FD_SET(w1_ask_read, &rfds);
    FD_SET(r1_ask_read, &rfds);

    
    /* Constants */
    char msg_no[80], msg_yes[80];
    msg_no[0] = 'N';
    msg_yes[0] = 'Y';
    bool has_changed[2] = {false, false};
    int cell[2] = {0,0}, temp_cell[2] = {0, 0};
    


    while (1)
    {
        char output_msg[80], msg_read[80], msg_write[80], read_str[80];
        int retval, bytes_read;

        struct timeval tv;
        tv.tv_sec = 5; // It is a good practice to reainitialize timout each time select is invoked
        tv.tv_usec = 0;

        
        


        retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &rfds))
            {
                if (i == w0_ask_read)
                {
                    // Read msg
                    read(i, read_str, sizeof(read_str));

                    if (read_str[0] == 'A')
                    {
                        printf("Writer0 asks to write \n");
                        if (cell[0] <= cell[1])
                        {
                            printf("Writer0 can write\n");
                            write(w0_ans_write, msg_yes, sizeof(msg_yes));
                        }
                        else
                        {
                            printf("Writer0 cannot write\n");
                            write(w0_ans_write, msg_no, sizeof(msg_no));
                        }
                    }
                    else
                    {
                        sscanf(read_str, "%i", &temp_cell[0]);
                        printf("Writer 0 set cell 0 to %i \n", temp_cell[0]);
                        has_changed[0] = true;
                    }
                }
                else if (i == r0_ask_read)
                {
                    read(i, read_str, sizeof(read_str));

                    if (read_str[0] == 'A')
                    {
                        printf("writer1 wants to write\n");
                        if (cell[1] <= cell[0])
                        {
                            printf("Writer 1 can write\n");
                            write(w1_ans_write, msg_yes, sizeof(msg_yes));
                        }
                        else
                        {
                            printf("Writer 1 cannot write\n");
                            write(w1_ans_write, msg_no, sizeof(msg_no));
                        }
                    }
                    else
                    {
                        sscanf(read_str, "%i", &temp_cell[1]);
                        printf("Writer1 set cell 1 to %i \n", temp_cell[1]);
                        has_changed[1] = true;
                    }
                }
                else if (i == r0_ask_read)
                {
                    read(i, read_str, 80);
                    if (read_str[0] == 'A')
                    {
                        printf("Reader 0 wants to read\n");
                        if (has_changed[0])
                        {
                            printf("Sent %i to reader 0 \n", temp_cell[0]);
                            sprintf(output_msg, format_string, temp_cell[0]);
                            write(r0_ans_write, output_msg, sizeof(output_msg));
                            has_changed[0] = 0;
                        }
                        else
                        {
                            printf("Value 0 has not changed\n");
                            write(r0_ans_write, msg_no, sizeof(msg_no));
                        }
                    }
                }
                else if (i == r1_ask_read)
                {
                    read(i, read_str, 80);
                    if (read_str[0] == 'A')
                    {
                        printf("Reader 1 wants to read\n");
                        if (has_changed[1])
                        {
                            printf("Sent %i to reader 1 \n", temp_cell[1]);
                            sprintf(output_msg, format_string, temp_cell[1]);
                            write(r1_ans_write, output_msg, sizeof(output_msg));
                            has_changed[1] = 0;
                        }
                        else
                        {
                            printf("Value 1 has not changed\n");
                            write(r1_ans_write, msg_no, sizeof(msg_no));
                        }
                    }
                }
            }
        }
    }
}