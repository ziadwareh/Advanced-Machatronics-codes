/**
 * @file shared_memory_API.c
 * @author Ziad Alwareh
 * @brief This is the explanation of the API functions used in this lecture
 * @version 0.1
 * @date 2024-01-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <semaphore.h>

/**
 * @brief This creates a new shared memory object inside a kernel. Please note that the newely created memory object is of size zero bytes.
 * we can make its size larger using the ftruncate system call.
 * 
 * @param name this is the name of the shared memory object
 * @param oflag This is a flag that controls the way the shared memory object is created. Could be:
 * 1. O_RDONLY: open the shared memory in read only mode
 * 2. O_WRONLY: open the shared memory in write only mode
 * 3. O_RDWR: open the shared memory in read-write mode
 * 4. O_CREAT: create a shared memory if it don't exist already
 * 5. O_APPEND: This appends new data to the existing content of the shared memory
 * 6. O_TRUNC: truncates the shared memory object
 * @return int that represents the file descriptor of the sharmed memory object
 */
int shm_open(const char *name, int oflag);

/**
 * @brief This creates a new shared memory object inside a kernel. Please note that the newely created memory object is of size zero bytes.
 * we can make its size larger using the ftruncate system call.
 * 
 * @param name this is the name of the shared memory object
 * @param oflag This is a flag that controls the way the shared memory object is created. Could be:
 * 1. O_RDONLY: open the shared memory in read only mode
 * 2. O_WRONLY: open the shared memory in write only mode
 * 3. O_RDWR: open the shared memory in read-write mode
 * 4. O_CREAT: create a shared memory if it don't exist already
 * 5. O_APPEND: This appends new data to the existing content of the shared memory
 * 6. O_TRUNC: truncates the shared memory object
 * @param mode the file permissions applied on this memory object
 * @return int that represents the file descriptor of the sharmed memory object
 */
int shm_open(const char *name, int oflag, mode_t mode);

/**
 * @brief This will resize the shared memory object to the size we want. This should be called after we use the shm_open() function
 * 
 * @param shm_fd this is the file descriptor of the shm object we want to enlrage.
 * @param new_size this is the new size in bytes that we want.
 * @return int -1 if it failed
 */
int ftruncate(int shm_fd, size_t new_size);

/**
 * @brief This mapes the memory segment in the kernel that we want to act as the shared memory into the process that is using the shared memory object.
 * 
 * @param addr the starting address of the process's virtual address space where the shm object shall begin to map the kernel data. recommended to pass as NULL.
 * this way the OS decides
 * @param length the length of the shared memory that should be mapped
 * @param prot PROT_READ | PROT_WRITE (specifies the protection mode. must be consistent with the access mode specified in the shm_open())
 * @param flags MAP_SHARED (this means that the updates to the mampped shared memory are visible to all other processes immediately)
 * @param fd this is the ds of the shared memory object
 * @param offset the location in the shared memory object at which the mapping starts
 * @return void* 
 */
void* mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

/**
 * @brief It destroyes the mapping between the shared memory objcet at location pointed by addr
 * 
 * @param addr address in process' virtual address space where the shared memory has been maped (starting at the end of the mapping). This is what is returned from the mmap()
 * @param length the lenght of the mapping which we want to unmap
 * @return int -1 if faliure. 0 if success.
 */
int munmap(void *addr, size_t length);

/**
 * @brief We are completely removing the shm object. This means if we try to create a new shm with the same name as the onld one, the processor won't complain
 * 
 * @param name name of the shm that we want to unlink
 * @return int -1 if faliure. 0 if success.
 */
int shm_unlink(const char* name);

/**
 * @brief decreases the reference count of the shared memory object being used. This lets the kernel know if it should free the resource if the reference count is zero or not
 * 
 * @param shm_fd the fd of the shm
 * @return int -1 if faliure. 0 if success.
 */
int close(int shm_fd);

/**
 * @attention Processes that access shared memory need to be synchronized. This means that we need to make sure that each process does not step on another process's
 * work in teh shared memory. This would require the use of something similar to condition variables, mutex, or a semaphore. These however are only suitable for ingle process communication.
 * To tackle this, we can use a modified version of the semaphore that is suitable for such an application. These are called named semaphores. They operate the same where where we 
 * use sem_post and sem_wait. the only difference is that when we create it the semaphore/
 */

/**
 * @brief This is used when we are creating a new named semaphore
 * 
 * @param name the name of the named semaphore
 * @param oflag This is a flag that controls the way the named semaphore object is created. Could be:
 * 1. O_RDWR: open the shared memory in read-write mode
 * 2. O_CREAT: create a shared memory if it don't exist already
 * 3. O_APPEND: This appends new data to the existing content of the shared memory
 * 4. O_EXCL: Ensure that the semaphore is created exclusively. If O_CREAT is also specified, and the semaphore already exists, sem_open fails.
 * @param mode permissions of the semaphore
 * @param value this is the initial count of the semaphore
 * @return sem_t* 
 */
sem_t* sem_open(const char *name, int oflag, mode_t mode, unsigned int value);

/**
 * @brief This is used when we are opening a pre-existing named semaphore
 * 
 * @param name the name of the named semaphore
 * @param oflag This is a flag that controls the way the named semaphore object is created. Could be:
 * 1. O_RDWR: open the shared memory in read-write mode
 * 2. O_CREAT: create a shared memory if it don't exist already
 * 3. O_APPEND: This appends new data to the existing content of the shared memory
 * 4. O_EXCL: Ensure that the semaphore is created exclusively. If O_CREAT is also specified, and the semaphore already exists, sem_open fails.
 * @return sem_t* 
 */
sem_t* sem_open(const char *name, int oflag);