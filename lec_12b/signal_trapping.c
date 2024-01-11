/**
 * @file signal_trapping.c
 * @author Ziad Alwareh
 * @brief this is to show how to detect a signal raised from OS to process. In here, we will handle a custom message to be displayed once 
 * ctl-C is hit to kill a program
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
/**
 * @attention The below code can catch a ctrl-c interrupt or abort if the user passed 'y'
*/
#include  <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void ctrl_c_handler(int sig){
    printf("\nCTRL-C pressed\n");
    printf("Sayonara Tetsero\n");
    exit(EXIT_SUCCESS);
}

static void abort_handler(int sig){
    printf("Process Aborted\n");
    printf("Sayonara Tetsero\n");
    exit(EXIT_SUCCESS);
}

int main(int argc,char **argv){
    signal(SIGINT, ctrl_c_handler); //SIGINT is the linux signal that is dedicated for interrupts. CTRL_C is considered an interrupt
    signal(SIGABRT,abort_handler); //SIGABRT is the linux signal that indicates an abort() command was called
    while (1==1){
        printf("Abort process (y/n) ?\n");
        char ch;
        scanf(" %c", &ch);
        if (ch == 'y'){
            abort();
        }
    }
    
}