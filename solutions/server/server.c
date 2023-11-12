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
    //placeholders for pipes to close
    int send_pipe_read[4];
    int receive_pipe_write[4];

    //pipes
    int send_pipe_writer_0;
    int receive_pipe_writer_0;
    int send_pipe_writer_1;
    int receive_pipe_writer_1;
    int send_pipe_reader_0;
    int receive_pipe_reader_0;
    int send_pipe_reader_1;
    int receive_pipe_reader_1;

    fd_set reading;
    char read_str[MSG_LEN];

    char ack_char[MSG_LEN]; 
    ack_char[0] = ACK_CHAR;

    char rej_char[MSG_LEN];
    rej_char[0] = REJ_CHAR;

    int cell[2] = {0,0};
    int tmp_cell[2] = {0,0};
    int has_changed[2] = {0,0};

    char format_string[MSG_LEN]="%d";
    char output_string[MSG_LEN];



    if(argc == 5){
        sscanf(argv[1], "%d %d %d %d", &receive_pipe_writer_0, &receive_pipe_write[0], &send_pipe_read[0], &send_pipe_writer_0);
        sscanf(argv[2], "%d %d %d %d", &receive_pipe_writer_1, &receive_pipe_write[1], &send_pipe_read[1], &send_pipe_writer_1);
        sscanf(argv[3], "%d %d %d %d", &receive_pipe_reader_0, &receive_pipe_write[2], &send_pipe_read[2], &send_pipe_reader_0);
        sscanf(argv[4], "%d %d %d %d", &receive_pipe_reader_1, &receive_pipe_write[3], &send_pipe_read[3], &send_pipe_reader_1);
    } else {
        printf("wrong args %i \n", argc); 
        sleep(15);
        return -1;
    }
    for(int i = 0; i < 4; i++){
        close(send_pipe_read[i]);
        close(receive_pipe_write[i]);
    }

    

    int fds[4];
    fds[0] = receive_pipe_writer_0;
    fds[1] = receive_pipe_writer_1;
    fds[2] = receive_pipe_reader_0;
    fds[3] = receive_pipe_reader_1;

    printf("Receiving on %d, %d, %d, %d \n", receive_pipe_writer_0, receive_pipe_writer_1, receive_pipe_reader_0, receive_pipe_reader_1);
    printf("Sending on %d, %d, %d, %d \n", send_pipe_writer_0, send_pipe_writer_1, send_pipe_reader_0, send_pipe_reader_1);


    int max_fd = -1;
    for(int i = 0; i < 4; i++){
        if(fds[i] > max_fd){
            max_fd = fds[i];
        }
    }

    printf("Max fd %i\n", max_fd);

    while (1) 
    {    
        //reset reading set
        FD_ZERO(&reading);
        FD_SET(receive_pipe_writer_0, &reading);
        FD_SET(receive_pipe_writer_1, &reading);
        FD_SET(receive_pipe_reader_0, &reading);
        FD_SET(receive_pipe_reader_1, &reading);
        int ret_val = select(max_fd + 1, &reading, NULL, NULL, NULL);

        for(int i = 0; i < max_fd + 1; i++){
            if(FD_ISSET(i, &reading)){
                if(i == receive_pipe_writer_0)
                {
                    read(i, read_str, MSG_LEN);

                    if (read_str[0] == ASK_CHAR){
                        printf("writer 0 wants to write\n");
                        if(cell[0] <= cell[1]){
                            printf("Writer 0 can write\n");
                            write(send_pipe_writer_0, ack_char, strlen(ack_char) + 1); 
                        }
                        else
                        {
                            printf("Writer 0 cannot write\n");
                            write(send_pipe_writer_0, rej_char, strlen(rej_char) + 1); 
                        }
                    } else {
                        sscanf(read_str, "%i", &tmp_cell[0]);
                        printf("Writer 0 set cell 0 to %i \n", tmp_cell[0]);
                        has_changed[0] = 1;
                    }
                    
                }
                else if (i == receive_pipe_writer_1)
                {
                    read(i, read_str, MSG_LEN);

                    if (read_str[0] == ASK_CHAR){
                        printf("writer 1 wants to write\n");
                        if(cell[1] <= cell [0]){
                            printf("Writer 1 can write\n");
                            write(send_pipe_writer_1, ack_char, strlen(ack_char) + 1); 
                        }
                        else
                        {
                            printf("Writer 1 cannot write\n");
                            write(send_pipe_writer_1, rej_char, strlen(rej_char) + 1); 
                        }
                    } else {
                        sscanf(read_str, "%i", &tmp_cell[1]);
                        printf("Writer 1 set cell 1 to %i \n", tmp_cell[1]);
                        has_changed[1] = 1;
                    }
                }
                else if (i == receive_pipe_reader_0)
                {
                    read(i, read_str, MSG_LEN);
                    if (read_str[0] == ASK_CHAR){
                        printf("Reader 0 wants to read\n");
                        if(has_changed[0]){
                            printf("Sent %i to reader 0 \n", tmp_cell[0]);
                            sprintf(output_string, format_string, tmp_cell[0]);
                            write(send_pipe_reader_0, output_string, strlen(output_string) + 1);
                            has_changed[0] = 0;
                        }
                        else
                        {
                            printf("Value 0 has not changed\n");
                            write(send_pipe_reader_0, rej_char, strlen(rej_char) + 1); 
                        }
                    }
                }
                else if (i == receive_pipe_reader_1)
                {
                    read(i, read_str, MSG_LEN);
                    if (read_str[0] == ASK_CHAR){
                        printf("Reader 1 wants to read\n");
                        if(has_changed[1]){
                            printf("Sent %i to reader 1 \n", tmp_cell[1]);
                            sprintf(output_string, format_string, tmp_cell[1]);
                            write(send_pipe_reader_1, output_string, strlen(output_string) + 1);
                            has_changed[1] = 0;
                        }
                        else
                        {
                            printf("Value 1 has not changed\n");
                            write(send_pipe_reader_1, rej_char, strlen(rej_char) + 1); 
                        }
                    }
                }
            }
        }
        
        cell[0] = tmp_cell[0];
        cell[1] = tmp_cell[1];
     } 
    return 0; 
} 