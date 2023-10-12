/**
 * @file condition_variables.c
 * @author Ziad Al Wareh
 * @brief This is a file in order to learn how to properly use the conditional variables. These
 * variables will block threads until a specific condition becomes true.
 * @version 0.1
 * @date 2023-10-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

//creates the variables itself
//pthread_cond_t cv;

/**
 * @brief This is used to initialize the conditional variable
 * 
 * @param cv is the pointer to the conditional variable
 * @param attr this is the attribute of the conditional varialbe (usually NULL)
 * @return int 
 */
//int pthread_cond_init(pthread_cond_t *cv, const pthread_condattr_t *attr);

/**
 * @brief when a thread is blocked by a condition variable, it should occure under the protection
 * of a mutex, meaning this should occure inside a mutex and once the thread is blocked, the mutex
 * is released and later obtained back by that node once the condition variable becomes true.
 * 
 * @param cv is the pointer to the conditional variable
 * @param mutex the mutex to release while blocked
 * @return int 
 */
//int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex);

/**
 * @brief Once a thread should wake up, it should be signaled. This function does that.
 * incase there are multiple threads that were blocked, there is no guarantee to which one
 * will wake up first.
 * 
 * @param cv is the pointer to the conditional variable to be signaled
 * @return int 
 */
//int pthread_cond_signal(pthread_cond_t *cv);

/**
 * @brief This is an example that would utilize the conditional variables. In here we will
 * be creating consumers and producers. The consumers cannot consume if the buffer is empty and
 * the producers cannot produce if it was full.
 * 
 */

#include <stdio.h>
#include <pthread.h>
// #include <stdlib.h>
#include <unistd.h>

//defining a buffer size
#define BUFFER_SIZE 4

//decleared an array of size 4 and an int variable with value 0
int theArray[BUFFER_SIZE], size =0;

//declaring the mutex variable
pthread_mutex_t myMutex;

//declaring the condition variable
pthread_cond_t myCV;

void *produce(void *arg){
    int id = (int) arg;
    int i;
    for (i=0;i<5; i++){
        //obtain the mutex
        pthread_mutex_lock(&myMutex);
        if (size == BUFFER_SIZE){
            printf("Producer %d wating ...\n", id);
            //block thread and wait for the condition variable to become true
            pthread_cond_wait(&myCV,&myMutex);
        }
        //add an entry to the array
        theArray[size] = i;
        printf("producer %d added %d\n", id, i);
        size ++;

        if(size ==1){
            //the array is not empty so we can signal consumers to consume
            pthread_cond_signal(&myCV);
        }

        //unlock the mutex
        pthread_mutex_unlock(&myMutex);
        //sleep for 1 millisecond for other threads to run
        usleep(1000);
    }
}

void *consume(void *arg){
    int item;
    int i;
    for (i=0;i<10; i++){
        //obtain the mutex
        pthread_mutex_lock(&myMutex);
        if (size == 0){
            printf("consumer wating ...\n");
            //block thread and wait for the condition variable to become true
            pthread_cond_wait(&myCV,&myMutex);
        }
        //obtain the last element from the array
        item = theArray[size-1];
        printf("consumer removed %d\n", item);
        size --;

        if(size ==BUFFER_SIZE-1){
            //the array is not full so we can signal producers to produce
            pthread_cond_signal(&myCV);
        }

        //unlock the mutex
        pthread_mutex_unlock(&myMutex);
        //sleep for 1 millisecond for other threads to run
        usleep(1000);
    }
}

int main(int argc, char* argv[]){
    //these will be the arguments to the custom producer function arguments
    int t1=1,t2=2;
    //the identifiers
    pthread_t consumer,producer1,producer2;

    //initializing the mutex and condition variable
    pthread_mutex_init(&myMutex,NULL);
    pthread_cond_init(&myCV,NULL);

    //create the pthreads hemselves
    pthread_create(&consumer,NULL,consume,NULL);
    pthread_create(&producer1,NULL,produce,(void *)t1);
    pthread_create(&producer2,NULL,produce,(void *)t2);

    //wait for the threads to finish their run
    pthread_join(consumer,NULL);
    pthread_join(producer1,NULL);
    pthread_join(producer2,NULL);

    //clean up before termination

    pthread_mutex_destroy(&myMutex);
    pthread_cond_destroy(&myCV);
    pthread_exit(NULL);

    return 0;
}
