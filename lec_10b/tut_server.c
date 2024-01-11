#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MAX_MSG_SIZE 256
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE+10)
#define SERVER_QUEUE_NAME "/sp-example-server"

mqd_t server_fd;
mqd_t client_fd;

char buffer[MSG_BUFFER_SIZE];

int
main(int argc, char *argv[]){
    int token = 1;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    //cerate the queue of the server
    server_fd = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT | O_EXCL, QUEUE_PERMISSIONS,&attr);
    if(server_fd == -1){
     printf("Couldn't create server mq");
     exit(EXIT_FAILURE);
    }
    for(;;){
        memset(buffer, 0, MSG_BUFFER_SIZE);
        if(mq_receive(server_fd, buffer, MSG_BUFFER_SIZE, NULL)==-1){
            printf("Fatal error occured");
            exit(EXIT_FAILURE);
        }
        //we now obtained the name of the mq we want to send a token to
        client_fd = mq_open(buffer, O_WRONLY);
        if(client_fd == -1){
            printf("couldn't open client mq");
            continue;
        }
        //send the token data
        memset(buffer, 0, MSG_BUFFER_SIZE);
        sprintf(buffer,"%d", token);
        if(mq_send(client_fd,buffer, strlen(buffer)+1,0)==-1){
            printf("couldn't send message");
            continue;
        }
        token ++;
        printf("Server responded to client");
    }
}