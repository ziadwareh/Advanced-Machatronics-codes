/**
 * @file concurrency.c
 * @author Ziad Al Wareh (you@domain.com)
 * @brief This is in order to learn more about the concurrency feature and how to use it
 * It is similar to the FreeRTOS concept. For this we will be using the POSIX API. They provide
 * Pthreads which provides the threading API. It is included using the pthreads.h
 * @version 0.1
 * @date 2023-10-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/**
 * @brief This is a function that creates a new thread that starts by executing the 
 * function indicated by start_routine
 * 
 * @param thread It is an identifier of the new thread
 * @param attr attributes of the thread - usually NULL
 * @param start_routine the name of the function that the thread starts executing
 * @param arg the argument to be passed to the start routine (only one)
 * @return int 
 */
//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

/**
 * @brief The thread that calls this function blocks its own execution until the
 * thread indicated by the parameter thread terminates its execution through either
 * finishing the function it started with or issued a pthread_exit() command
 * 
 * @param thread identification of teh thread to wait for
 * @param status the exit status of the terminating thread. It can be NULL
 * @return int 
 */
//int pthread_join(pthread_t thread, void *status);

/**
 * @brief This is the funciton prototype of pthread in order to exit the branch that calls it
 * 
 * @param status the exit status of the thread. This is passed to the status variable of the
 * pthread_join() function of a thread that is waiting for it to exit.
 */
//void pthread_exit(void* status);

/**
 * @brief Now that we have explained how everything is working, we will be creating and terminating
 * some threads as experimentation.
 * 
 */

#include <pthread.h>
#include <stdio.h>

//created the attribute variable for teh create pthread function
pthread_attr_t attr;

void *threadDefault(void *args) {
    printf("A thread with the default attributes is created!\n\n");
    pthread_exit(NULL);
}

void *threadCustomized(void *arg){
    int policy;
    int detachstate;
    printf("A thread with customized attributes is created!\n");

    /* This is used to get the detachstate of a thread. the detach state
        determines whether it is a joinable thread or a detached thread.
        @param &attr this is a pointer to the attribute variable of the thread we want
        to get the detach state of.
        @param &detachstate this is a pointer to the variable we want to store the returned
        output in. It will be set to either PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHED.
         */
    pthread_attr_getdetachstate(&attr, &detachstate);

    printf(" Detach state: %s\n",
       (detachstate == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
       (detachstate == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
       "???");

    /*used to retrieve the scheduling policy attribute of a thread attribute object. 
    It allows you to determine the scheduling policy that will be used when a thread
     is created with the specified attributes.
     @param &attr this is a pointer to the attribute variable of the thread we want
     to get the detach state of.
     @param &policy  A pointer to an integer where the function will store the current 
     scheduling policy. This integer will be set to one of the scheduling policy constants,
      such as SCHED_FIFO, SCHED_RR, or SCHED_OTHER*/
    pthread_attr_getschedpolicy(&attr, &policy);

    printf(" Scheduling Policy: %s\n",
        (policy == SCHED_OTHER) ? "SCHED_OTHER" :
        (policy == SCHED_FIFO) ? "SHED_FIFO" :
        (policy == SCHED_RR) ? "SCHED_RR" :
        "???");

        pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    //The thread identifiers for two threads
    pthread_t threadD, threadC;
    int rc;

    /*Initialize the attributes this can later be configured using different functions to
    set the different parameters of the attribute if it was needed. This alone sets it
    with the default attributes. If this function returns 0 then it succeded, else it failed*/
    rc = pthread_attr_init(&attr);

    if (rc)
        printf("ERROR; RC from pthread_attr_init() is %d \n", rc);
    
    /*making the attribute of the thread indicate that it is a detachable state. Therefore
    we are setting the detachstate of the attribute of the thread. If this function returns 0
    then it succeded, else it failed*/
    rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (rc)
        printf("ERROR; RC from pthread_attr_setdetachstate() is %d \n", rc);
    
    /*creating a thread with a default funciton. If this function returns 0
    then it succeded, else it failed*/
    rc = pthread_create(&threadD, NULL, threadDefault, NULL);
    if (rc)
        printf("ERROR when creating default thread, Code is %d \n", rc);

    /*creating a thread with a customized funciton. If this function returns 0
    then it succeded, else it failed*/
    rc = pthread_create(&threadC, &attr, threadCustomized, NULL);
    if (rc)
        printf("ERROR when creating customzied thread, Code is %d \n", rc);

    /* Join the threads. I think this makes sure that the process would wait for the branches to
    finish before terminating the proccess*/
    pthread_join(threadD, NULL);
    pthread_join(threadC, NULL);

    /*Free up the attribute object and exit*/
    /*The pthread_attr_destroy function is used to destroy (or clean up) a previously
    initialized pthread attribute object (pthread_attr_t) after you have finished using it.
    This function is typically called to release any resources associated with the attribute
    object.*/
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}
