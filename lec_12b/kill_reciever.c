/**
 * @file signal_trapping.c
 * @author Ziad Alwareh
 * @brief this is to show how to receive a kill signal from a process 
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MQ_NAME "/sp-example-server"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE+10)

mqd_t msgq;
char buffer[MSG_BUFFER_SIZE];
int counter = 1;

int main(){

    //display our pid of this process and send it using a message queue
    pid_t pid = getpid();
    printf("pid is %d\n",pid);

    //create the mq
    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = MAX_MSG_SIZE;

    msgq = mq_open(MQ_NAME,O_WRONLY|O_CREAT,0660,&attr);

    if(msgq ==-1){
        printf("An error occured while creating queue");
        exit(EXIT_FAILURE);
    }

    memset(buffer,0,MSG_BUFFER_SIZE);
    sprintf(buffer,"%d",pid);
    printf("buffer data is:  %s \n",buffer);
    if(mq_send(msgq,buffer,strlen(buffer)+1,0)==-1){
        printf("An error occured while sending message");
        exit(EXIT_FAILURE);
    }
    while(1);


}