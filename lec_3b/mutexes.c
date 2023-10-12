/**
 * @file mutexes.c
 * @author Ziad Alwareh
 * @brief This is in order to learn about mutexes and how to use them for syncronization of 
 * primitives
 * @version 0.1
 * @date 2023-10-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

//Creates a mutex
// pthread_mutex_t mutex

/**
 * @brief: This initializes a mutex
 * 
 * @param mutex is a pointer to the mutex object that i would like to initalize, that is why we 
 * declare it first usually and pass its address to this function
 * @param attr this is the pointer to the mutex attributes that are set during the initalization.
 * If we want default attributes, then use NULL, else, if you want to set the attribues to something
 * specific, then create an object of type pthread_mutexattr_t, set its parameters, then pass its 
 * address to this function
 * @return int 0 if it was successful, else it was not
 */
//int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

/**
 * @brief The thread calling this function will try to get exclusive access to the mutex, if
 * the mutex was locked, then the thread will block and wait until it can gain access. If multiple
 * threads were trying to gain access to the mutex, then they will recieve it based on their
 * priority
 * 
 * @param mutex is the mutex that we want to gain exclusive access to
 * @return int 0 if it was successful
 */
//int pthread_mutex_lock(pthread_mutex_t *mutex);

/**
 * @brief When a thread is done utilizing the mutex, it should unlock it so other threads can 
 * access it. This is when this function is used.
 * 
 * @param mutex is the mutex we want to release
 * @return int 0 if it was successful
 */
//int pthread_mutex_unlock(pthread_mutex_t *mutex);

/**
 * @brief we will now create an algorithm that creates 2 threads. One will be a thread that
 * tries to deposite money into an account, while another will try to withdraw money.
 * 
 */

#include <pthread.h>
#include <stdio.h>

double balance = 0;

pthread_mutex_t account;

void *deposit(void *dummy){
    double credit;
    pthread_mutex_lock(&account);
    printf("\nI am in thread 1, Enter amount to deposit: ");
    scanf("%lf",&credit);
    balance += credit;
    printf("The new balance is: %lf\n",balance);
    pthread_mutex_unlock(&account);
    pthread_exit(NULL);
}

void *withdraw(void *dummy){
    double debit;
    pthread_mutex_lock(&account);
    printf("\nI am in thread 2, Enter amount to withdraw: ");
    while(1){
        scanf("%lf",&debit);
        if(balance - debit < 0)
            printf("Insufficient balance. Please enter a smaller amount: ");
        else
            break;
    }

    balance -= debit;
    printf("The new balance is: %lf\n",balance);
    pthread_mutex_unlock(&account);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    pthread_t thread1, thread2;
    int rc;

    rc = pthread_create(&thread1,NULL,deposit,NULL);
    if (rc)
         printf("ERROR when creating thead1, Code is %d \n", rc);

    rc = pthread_create(&thread2,NULL,withdraw,NULL);
    if (rc)
         printf("ERROR when creating thead2, Code is %d \n", rc);
    
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_mutex_destroy(&account);
    pthread_exit(NULL);
}