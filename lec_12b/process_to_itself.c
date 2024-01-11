/**
 * @file signal_trapping.c
 * @author Ziad Alwareh
 * @brief this is to show how to a process can send a singal to itself using raise
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include  <signal.h>
#include <stdio.h>
#include <stdlib.h>

void signal_catchfunc(int sig){
    printf("!! signal cought !\n");
    abort();
}

int main(){
    //creating the signal handler
    if (signal(SIGINT,signal_catchfunc) == SIG_ERR){
        printf("Error: unable to set signal handler\n");
        exit(EXIT_FAILURE);
    }
    printf("Raising a signal \n");
    //raising an interupt signal
    if(raise(SIGINT) !=0){
        printf("Error: unable to raise signal");
        exit(EXIT_FAILURE);
    }
    //if this didn't exit, then the signal_catchfunc will be triggered to display the statement and kill
}