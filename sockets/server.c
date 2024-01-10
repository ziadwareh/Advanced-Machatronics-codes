#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/**
 * @brief these are the name of the socket we want to create as well as the size of the socket's buffer
 * 
 */
#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int
main(int argc, char *argv[])
{
    /**
     * @brief This is a pre-defined struct included in the socket.h library. The #if 0 part is a fancy way of commenting out
     * the lines included within it. The purpose is to show what the struct sockaddr_un includes as parameters
     * 
     */
    struct sockaddr_un name;
    
#if 0  
    struct sockaddr_un {
        sa_family_t sun_family;               /* AF_UNIX */
        char        sun_path[108];            /* pathname */
    };
#endif
    //This will be later used in order to listen to errors that might pop while creating the sockets
    int ret;
    //This will be used to hold the master socket that we are about to create
    int connection_socket;
    //This will contian the client handler descriptors later on
    int data_socket;
    //This will contain the accumelated sent data from a client
    int result;
    //This will include the data sent in each message from the client
    int data;
    //This is the buffer that determines the size of a single message sent from a client
    char buffer[BUFFER_SIZE];

    /*In case the program exited inadvertently on the last run,
     *remove the socket.
     **/

    /**
     * @brief This unlinkes the socket incase its name existed. This is performed incase of an unclean shutdown occured that resulted
     * in the socket not being completely terminated. The unlink removed the socket file from the file system
     * 
     */
    unlink(SOCKET_NAME);

    /* Create Master socket. */

    /*SOCK_DGRAM for Datagram based communication*/

    /**
     * @brief This creates the communication socket. 
     * 
     * The first parameter specifies the type of socket, in our case, UNIX domain sockets.
     * This is needed for the referencing and naming conventions. AF_UNIX specifies that communication will be held on the same machine, 
     * within the same host. 
     * 
     * The SOCK_STREAM specifies the communication type. In this case, the communication type was set to STREAM therefore,
     * a reliable connection is first initiated before data is transfered.
     * 
     * Finally the 0 here sets the protocol and in this case, 0 means that the system should choose the defauly communication protocole for
     * the specified address family and socket type.
     * 
     * This function should return either the value of the socket created or a -1 in case of an error
     * 
     * P.S. This is the master socket that any socket we create will be binded to it. It will be responsible for creating client handlers
     * 
     */
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    /**
     * @brief Handling error in case socket creation faliure
     * 
     */
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    printf("Master socket created\n");

    /*initialize*/
    /**
     * @brief The memset function sets all the bytes in the name variable of type struct sockaddr_un to the passed parameter (in this case 0)
     * in order to make sure that there are no left over values in the name variable to properly initialize it
     * 
     */
    memset(&name, 0, sizeof(struct sockaddr_un));

    /*Specify the socket Cridentials*/
    /**
     * @brief Set the address family type of the name variable. This matches that of the master socket
     * 
     */
    name.sun_family = AF_UNIX;
    /**
     * @brief This copies the SOCKET_NAME into the sun_path parameter of the name variable. It is copying at most sizeof(name.sun_path) - 1
     * characters from SOCKET_NAME to guarentee that there is space for a null terminator at the end of the string. This is required. A null
     * terminator is how string end in C and it is '\0'. There must be room for it to be added after we use the strncpy since the size of the
     * sun_path is already set to be 120 at most, so we want to leave room for the terminator to avoid errors.
     * 
     */
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /* Bind socket to socket name.*/
    /* Purpose of bind() system call is that application() dictate the underlying 
     * operating system the criteria of recieving the data. Here, bind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket", 
     * then such data needs to be delivered to this server process (the server process)*/

    /**
     * @brief This is linking the master socket to the address we created. The address we created was contained in the name variable.
     * The bind function accepted the connection_socket variable, which contains the master socket we created. it also accepted a pointer
     * to the name variable in order to to assign the master socket to the address specified by the name variable. We finally specify the
     * size of the address structure being passed. This function will return 0 in case it was successful, else -1
     * 
     */
    ret = bind(connection_socket, (const struct sockaddr *) &name,
            sizeof(struct sockaddr_un));

    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("bind() call succeed\n");
    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     * */
    
    /**
     * @brief This will put the socket into a listening state for incomming connections. it accepts the mater socket and an int. In this case
     * the int is 20. This means that we can have up to 20 clients queued waiting to be serviced. This function will return 0 in case it was successful, else -1
     * 
     */
    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */
    /*All Server process usually runs 24 x 7*/
    for (;;) {

        /* Wait for incoming connection. */
        printf("Waiting on accept() sys call\n");

        /**
         * @brief This accepts incomming client connection requests that might occur from the listen state of the socket. It accepts the
         * master socket we created as a parameter. The two NULL parameters are usually set to addresses incase we are interested in obtaining
         * the client's address. In this case we are not interested, therefore it is set to NULL. The data_socket variable will be our client handler
         * This will either contain the data of the file descripter of the client handler or -1 in case of faliure.
         * 
         */
        data_socket = accept(connection_socket, NULL, NULL);

        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        printf("Connection accepted from client\n");

        result = 0;
        for(;;) {

            /*Prepare the buffer to recv the data*/
            /**
             * @brief This clears the data in the data buffer
             * 
             */
            memset(buffer, 0, BUFFER_SIZE);

            /* Wait for next data packet. */
            /*Server is blocked here. Waiting for the data to arrive from client
             * 'read' is a blocking system call*/
            printf("Waiting for data from the client\n");
            /**
             * @brief This reads the data from the client into the buffer. This function will return 0 in case it was successful, else -1
             * 
             */
            ret = read(data_socket, buffer, BUFFER_SIZE);

            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Add received summand. */
            /**
             * @brief This will copy the data of the buffer into the data variable. It takes the address of the data variable, the buffer data
             * and the number of bytes it needs to copy.
             * 
             */
            memcpy(&data, buffer, sizeof(int));
            /**
             * @brief The communication is set so that whenever the client sends a 0, terminate the connection
             * 
             */
            if(data == 0) break;
            /**
             * @brief Add the extracted data to the overall results variable that contains all the accumulated data.
             * 
             */
            result += data;
        }

        /* Send result. */
        /**
         * @brief Clear the buffer
         * 
         */
        memset(buffer, 0, BUFFER_SIZE);
        /**
         * @brief set the data of the buffer to be "Result = result"
         * 
         */
        sprintf(buffer, "Result = %d", result);

        printf("sending final result back to client\n");
        /**
         * @brief Send the data of the buffer to the client through the client handler. This function will return 0 in case it was successful, else -1
         * 
         */
        ret = write(data_socket, buffer, BUFFER_SIZE);
        if (ret == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        /* Close socket. */
        /**
         * @brief This terminates the data handler of that client ending the connection between it and the server
         * 
         */
        close(data_socket);
    }

    /*close the master socket*/
    /**
     * @brief This closes the master socket terminating it
     * 
     */
    close(connection_socket);
    printf("connection closed..\n");

    /* Server should release resources before getting terminated.
     * Unlink the socket. */
    
    /**
     * @brief This removes the file descriptor of the socket we had created to guarentee a clean shutdown
     * 
     */
    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}