/**
 * @file message_queues.c
 * @author Ziad Alwareh
 * @brief This file is in order to know how to use message queues as an IPC technique
 * @version 0.1
 * @date 2024-01-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/**
 *
 * Message queues (mq) are kernel processes. This means that the kernel manages storing, creating, and destroying it. Each one has its own id
 * which is its name and it must be unique. Once a queue is created, the kernel keeps count of every process that is using it. Once all the processes
 * close it, then the counter of processes would be zero, the kernel would automatically free the resource via destroying it
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

//This is a message queue file descriptor object that will hold the message queue's file descriptor
mqd_t msgq;

/**
 * @brief This creates a message queue
 * 
 * @param name This is the reference name of the created queue. It must start with '/' and cannot contain an additional '/'
 * @param oflag This is a flag that controls the way the msg_queue is opened. Could be:
 * 1. O_RDONLY: read only (for recieving messages)
 * 2. O_WRONLY: write only (for sending messages)
 * 3. O_RDWR: for both sending and receiving (read and write)
 * 4. O_CREAT: The queue is created if it does not exist
 * 5. E_EXCL: used with the O_CREAT. This will trigger when the mq_open fails. This happens when we try to create an existing mq.
 * 6. O_NONBLOCK: will not block a process if it was unable to access the queue.
 * @return mqd_t which is the file descriptor of the mq. if it returned -1, then an error occured
 */
mqd_t mq_open(const char *name, int oflag);

/**
 * @brief 
 * 
 * @param name his is the reference name of the created queue. It must start with '/' and cannot contain an additional '/'
 * @param oflag This is a flag that controls the way the msg_queue is opened. Could be:
 * 1. O_RDONLY: read only (for recieving messages)
 * 2. O_WRONLY: write only (for sending messages)
 * 3. O_RDWR: for both sending and receiving (read and write)
 *mq_a 4. O_CREAT: The queue is created if it does not exist
 * 5. E_EXCL: used with the O_CREAT. This will trigger when the mq_open fails. This happens when we try to create an existing mq.
 * 6. O_NONBLOCK: will not block a process if it was unable to access the queue.
 * @param mode these are permissions set by the owning process. This is usually set to 0660
 * @param attr specifies various attributes of the mq
 * @return mqd_t which is the file descriptor of the mq. if it returned -1, then an error occured
 */
mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

struct mq_attr{
    long mq_flags; /*Flags: 0*/
    long mq_maxmsg; /*Nax number of messages on queue*/
    long mq_msgsize; /*Mas message size in bytes*/
    long mq_curmsgs; /*number of current messages in the queue*/
}

/**
 * @brief This would close the message queue for the current using process
 * 
 * @param msgQ this is the message queue that we want to close
 * @return int 0 if success & -1 if failure
 */
int mq_close(mqd_t msgQ);

/**
 * @brief It addes the message indicated by msg_ptr to the queue. If the queue is full, the caller will be blocked until it can queue unless the
 * O_NONBLOCK flag is used in the mq oflag parameter
 * 
 * @param msgQ This is the file descriptor of the mq we want to use
 * @param msg_ptr points to the buffer of the message we want to queue
 * @param msg_len the size of the message we want to queue. It should be less than or equal to the mq_maxmsg attribute variable
 * @param msg_prio this is the priority of the message. messages of the same priority are stored in the queue in FIFO order.
 * @return int 0 if success & -1 if failure
 */
int mq_send(mqd_t msgQ, char *msg_ptr, size_t msg_len, unsigned int msg_prio);

/**
 * @brief This dequeues a message from the queue. If the queue was empty, the caller gets blocked unless the O_NONBLOCK flag is used. The message with
 * the highest priority is retrieved first
 * 
 * @param msgQ This is the file descriptor of the mq we want to use
 * @param msg_ptr points to the empty message buffer that we will store the data in
 * @param msg_len this is the size of the buffer in bytes
 * @param msg_prio stores the priority of the message. If we don't care about it, we can set this to NULL.
 * @return int 0 if success & -1 if failure
 */
int mq_receive(mqd_t msgQ, char *msg_ptr, size_t msg_len, unsigned int msg_prio);

/**
 * @brief Destroyes the mq. should not be used if there are otehr processes using the mq.
 * 
 * @param msgQ_name id of the mq we want to destroy
 * @return int 0 if success & -1 if failure
 */
int mq_unlink(const char* msgQ_name);