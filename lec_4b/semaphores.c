/**
 * @file semaphores.c
 * @author Ziad Al Wareh
 * @brief This is a file that will explain semaphores which are used to allow different threads
 * to acces the same resource. we can set the number of threads that can access at the same time.
 * If we exceed that, the thread trying to access will block itself. Once there is availability,
 * one of the blocked threads will unblock. There is no guarentee to which one will it be. 
 * **NOTE**a semaphore can be used as a mutex if we set its value to 1.
 * 
 * @version 0.1
 * @date 2023-10-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

//variable to initialize the semaphore
//sem_t sem;

/**
 * @brief This initializes the semaphore
 * 
 * @param sem is the semaphore we want to initialize
 * @param pshared indicates how the semaphore is shared. This is whether or not it is meant to
 * be used among multiple processes or is it intended for threads within the same process. if 
 * this was non-zero, then it is multi-process. If zero, it is single process.
 * 
 * @param value the intial value of the semaphore which determines how many threads can join
 * @return int 
 */
//int sem_init(sem_t *sem, int pshared, unsigned int value);

/**
 * @brief This would decrement the value of the semaphore. If the value of the semaphore was
 * greater than zero, then this function will return immediatly, otherwise, it will block the
 * calling thread until the value of the semaphore becomes greater than zero.
 * 
 * @param sem the semaphore we want to decrement
 * @return int 
 */
//int sem_wait(sem_t *sem);

/**
 * @brief This would incerment the semaphore value by 1. This will inturn unblock blocked threads
 * since there is now an availability in the semaphore.
 * 
 * @param sem the semaphore we want to increment
 * @return int 
 */
//int sem_post(sem_t *sem);

/**
 * @brief this is now an example on how to use a semaphore. we will be creating readers and writters
 * when someone is writting, no one can read or write. but multiple readers can read.
 * 
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//define number of readers and writters
#define READERS 5
#define WRITERS 4

//define sleep times
#define READER_SLEEP_TIME 20000
#define WRITER_SLEEP_TIME 50000
#define MAIN_SLEEP_TIME 5000000

sem_t semREAD;
sem_t semWRITE;
int readCount;

void *reader(void *arg){
    int *p = (int *)arg;
    while(1){
        //lock the semaphore in order to be able to updated the readCount safely
        sem_wait(&semREAD);
        readCount++;
        printf("Number of readers: %d\n",readCount);
        if (readCount==1){
            //lock the write semaphore so that no one writes while we read
            sem_wait(&semWRITE);
        }
        //release the semaphore since we changed the readcount safely
        sem_post(&semREAD);
        printf("Reader #%d reading ....\n",(int)*p);
        //lock semaphore to safely decrement readcount
        sem_wait(&semREAD);
        readCount--;
        //if there are no more readers, allow writers to write.
        if (readCount ==0){
            sem_post(&semWRITE);
        }
        sem_post(&semREAD);
        usleep(READER_SLEEP_TIME);
    }
}

void *writer(void *arg){
    int *p = (int *) arg;
    while(1){
        //block the write sem so we can safely write
        sem_wait(&semWRITE);
        printf("Writer #%d writing ....\n",(int)*p);
        //unblock the sem
        sem_post(&semWRITE);
        usleep(WRITER_SLEEP_TIME);
    }
}

int main(int argc, char* argv[]){
    //decleare the thread identifiers
    pthread_t readers[READERS];
    pthread_t writers[WRITERS];
    int i, rc, r[READERS],w[WRITERS];
    readCount = 0;
    //initialize the semaphores
    sem_init(&semREAD,0,1);
    sem_init(&semWRITE,0,1);
    //create the readers and writers threads
    for(i=0;i<WRITERS;i++){
        w[i] = i;
        rc = pthread_create(&writers[i],NULL,writer,(void *)&w[i]);
        if (rc){
            perror("In writer pthread_create()");
            exit(1);
        }
        usleep(20000);
    }
    for(i=0;i<READERS;i++){
        r[i] = i;
        rc = pthread_create(&readers[i],NULL,reader,(void *)&r[i]);
        if (rc){
            perror("In reader pthread_create()");
            exit(1);
        }
        usleep(20000);
    }
    usleep(MAIN_SLEEP_TIME);

    //cancel all threads
    for(i=0;i<WRITERS;i++){
        pthread_cancel(writers[i]);
    }
    for(i=0;i<READERS;i++){
        pthread_cancel(readers[i]);
    }
    //destroy semaphores
    sem_destroy(&semREAD);
    sem_destroy(&semWRITE);

    return 0;
}