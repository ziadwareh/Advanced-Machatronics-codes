#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int
main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    /* Create data socket. */

    /**
     * @brief This creates a socket for the client. This matches the same type of socket of
     * the master socket. This is used in order to be compatible with the master once it
     * communicates with it.
     * 
     */
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     * */

    /**
     * @brief The memset function sets all the bytes in the addr variable of type struct sockaddr_un to the passed parameter (in this case 0)
     * in order to make sure that there are no left over values in the addr variable to properly initialize it
     * 
     */
    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    /**
     * @brief Setting the details of the address. Same as what we did in the server
     * 
     */
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    /**
     * @brief This establishes a connection with a socket. It is trying to communicate using the
     * data_socket which should contain the same address as the master socket that it wants to connect to.
     * 
     * The first argument is the file descriptor of the socket it had created that it wants to connect using. 
     * 
     * The second parameter is a pointer to the address of the server it wants to connect to. 
     * 
     * Finally, it takes the size of the address structure being passed.
     * 
     * This function will return 0 in case it was successful, else -1
     */
    ret = connect (data_socket, (const struct sockaddr *) &addr,
            sizeof(struct sockaddr_un));

    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    /* Send arguments. */
    /**
     * @brief this will ask us to write data that we want to send to the server. Once we write 0, the
     * connection with the server will be terminated
     * 
     */
    do{
        printf("Enter number to send to server :\n");
        scanf("%d", &i);

        /**
         * @brief Write data to the server via the created data socket.
         * 
         */
        ret = write(data_socket, &i, sizeof(int));
        if (ret == -1) {
            perror("write");
            break;
        }
        printf("No of bytes sent = %d, data sent = %d\n", ret, i); 
    } while(i);

    /* Request result. */
    /**
     * @brief Clears its buffer in order to recieve data from the server
     * 
     */
    memset(buffer, 0, BUFFER_SIZE);
    /**
     * @brief copies the string RES into the buffer
     * 
     */
    strncpy (buffer, "RES", strlen("RES"));
    /**
     * @brief Adds the null terminator to the buffer
     * 
     */
    buffer[strlen(buffer)] = '\0';
    /**
     * @brief Prints the content of the buffer
     * 
     */
    printf("buffer = %s\n", buffer);

    /**
     * @brief Sends the data of the buffer to the server
     * 
     */
    ret = write(data_socket, buffer, strlen(buffer));
    if (ret == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    /* Receive result. */
    memset(buffer, 0, BUFFER_SIZE);
    
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    /* Ensure buffer is 0-terminated. */

    buffer[BUFFER_SIZE - 1] = 0;

    printf("Result = %s\n", buffer);

    /* Close socket. */

    close(data_socket);

    exit(EXIT_SUCCESS);
}
