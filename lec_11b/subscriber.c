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

int main(int argc, char* argv[]){
    //open smh
    int shm_fd = shm_open(SHM_NAME,O_RDONLY,0666);
    if(shm_fd==-1){
        printf("error with shm_open()");
        exit(EXIT_FAILURE);
    }
    //map the kernel memory to the VAS
    void* ptr = mmap(NULL,SHM_SIZE,PROT_READ, MAP_SHARED,shm_fd,0);
    if(ptr == MAP_FAILED){
        printf("error with mmap()");
        exit(EXIT_FAILURE);
    }

    //read from the SMH
    char msg[MSG_SIZE];
    sprintf(msg, "%s",(char*)ptr);
    printf("Content in the smh: %s\n",msg);

    if(shm_unlink(SHM_NAME)==-1){
        printf("error with shm_unlink()");
        exit(EXIT_FAILURE);
    }

}