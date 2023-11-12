#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/select.h>
#include <unistd.h> 
#include <stdlib.h>
#include "include/constants.h"

int main(int argc, char *argv[]) 
{
    int send_pipe_read;
    int send_pipe;
    int receive_pipe;
    int receive_pipe_write;

    char ask_char[MSG_LEN]; 
    ask_char[0] = ASK_CHAR;
    char read_str[MSG_LEN];
    char ack_char = ACK_CHAR;
    char send_str[MSG_LEN];
    int send_int = 1;
    int writer_num = -1;

    if(argc == 3){
        sscanf(argv[1],"%i", &writer_num);
        sscanf(argv[2], "%d %d %d %d", &send_pipe_read, &send_pipe, &receive_pipe, &receive_pipe_write);
    } else {
        printf("wrong number of arguments\n"); 
        return -1;
    }

    int in_progress = 0;

    close(send_pipe_read);
    close(receive_pipe_write);

    printf("Writer number %i \n", writer_num);
    printf("Sending on pipe %i \n", send_pipe);
    printf("Receiving on pipe %i \n", receive_pipe);
    

    srandom(writer_num * SEED_MULTIPLIER);

    while (1) 
    {   
        // send ask
        if(!in_progress){
            printf("Sent write request\n");
            write(send_pipe, ask_char, strlen(ask_char) + 1);
            in_progress = 1;
        }
        else //wait for reply
        {
            read(receive_pipe, read_str, MSG_LEN);
            if(read_str[0] == ACK_CHAR)
            {
                send_int = random() % MAX_NUMBER;
                sprintf(send_str, "%i", send_int);

                printf("Acknowledged, sent %i \n", send_int);

                write(send_pipe, send_str, strlen(send_str)+1);
            }
            else if (read_str[0] == REJ_CHAR)
            {
                printf("Rejected \n");
            }
            in_progress = 0;
        }
        sleep(2);
    } 
    return 0; 
} 