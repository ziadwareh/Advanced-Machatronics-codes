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
char client_queue_name[MSG_BUFFER_SIZE];

char buffer[MSG_BUFFER_SIZE];

int
main(int argc, char **argv){

    if(argc<=1){
        printf("provide a reciepient msgQ name : format </msgq-name>\n");
        return 0;
    }
    //set the data of the buffer to be the name of the client's mq
    memset(buffer,0,MSG_BUFFER_SIZE);
    sprintf(buffer,"%s",argv[1]);
    strncpy(client_queue_name,buffer,MSG_BUFFER_SIZE);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    //create the client mq that the server will connect to
    client_fd = mq_open(buffer,O_RDONLY | O_CREAT | O_EXCL, QUEUE_PERMISSIONS,&attr);
    if (client_fd == -1){
        printf("error occured while created client mq");
        exit(EXIT_FAILURE);
    }

    server_fd = mq_open(SERVER_QUEUE_NAME,O_WRONLY);
    if(server_fd == -1){
        printf("error occured while connecting to server");
        exit(EXIT_FAILURE);
    }
    // send mq name to server
    if(mq_send(server_fd,buffer,strlen(buffer)+1,0)==-1){
        printf("error occured while sending date to server");
        exit(EXIT_FAILURE);
    }

    //recieve the token number
    memset(buffer,0,MSG_BUFFER_SIZE);
    if(mq_receive(client_fd, buffer, MSG_BUFFER_SIZE, NULL)==-1){
        printf("error occured while recieving token");
        exit(EXIT_FAILURE);
    }
    printf("client token is %s \n", buffer);

    if(mq_close(client_fd)==-1){
        printf("error occured while closing client_fd");
        exit(EXIT_FAILURE);
    }
    if(mq_unlink(client_queue_name)==-1){
        printf("error occured while unlinking client_fd");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}