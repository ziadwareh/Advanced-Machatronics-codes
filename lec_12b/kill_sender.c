/**
 * @file signal_trapping.c
 * @author Ziad Alwareh
 * @brief this is to show how to send a kill signal to a process for it to abort
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

mqd_t msgq;

#define MSG_BUFFER_SIZE (MAX_MSG_SIZE+10)

char buffer[MSG_BUFFER_SIZE];
int pid;

//we will be receiving the pid of the process using a mq

int main(){
    //open mq
    msgq = mq_open(MQ_NAME,O_RDONLY);
    if(msgq ==-1){
        printf("An error occured while creating queue");
        exit(EXIT_FAILURE);
    }
    //get the msg
    memset(buffer,0,MSG_BUFFER_SIZE);
    if(mq_receive(msgq,buffer,MSG_BUFFER_SIZE,NULL)==-1){
        printf("error while recieving data");
        exit(EXIT_FAILURE);
    }
    printf("pid of target process is%s\n",buffer);
    
    pid = atoi(buffer);
    kill(pid,SIGUSR1);
    // Close the message queue
    if (mq_close(msgq) == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }
    if(mq_unlink(MQ_NAME)==-1){
        printf("error while unlinking mq");
        exit(EXIT_FAILURE);
    }
    printf("terminated target successfully");
    exit(EXIT_SUCCESS);
}