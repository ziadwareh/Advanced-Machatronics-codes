#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHM_SIZE 4096
#define MSG_SIZE 100
#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"

sem_t* sem;
int main(int argc, char* argv[]){
    //create the names semaphore
    sem = sem_open(SEM_NAME,O_CREAT, 0664,1);

    //create the shm object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if(shm_fd==-1){
        printf("error with shm_open()");
        exit(EXIT_FAILURE);
    }
    
    //resize it to the desired size
    if(ftruncate(shm_fd,SHM_SIZE)==-1){
        printf("error with ftruncate()");
        exit(EXIT_FAILURE);
    }

    //map the allocated kernel space to the process' VAS
    void *ptr = mmap(NULL,SHM_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
    if(ptr == MAP_FAILED){
        printf("error with mmap()");
        exit(EXIT_FAILURE);
    }
    //write in the shm while making sure that its the only one able to write
    char msg[MSG_SIZE];
    printf("Insert message you want to add. \n");
    fgets(msg,MSG_SIZE,stdin);
    sem_wait(sem);
    sprintf(ptr,"%s",msg);
    sem_post(sem);

    printf("The message has been written to shm. \n");
    printf("         Content was: %s.\n",msg);

    //close the semaphore
    sem_close(sem);
    //destroy the semaphore
    sem_unlink(SEM_NAME);
}