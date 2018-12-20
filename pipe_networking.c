#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    // create WKP
    int WKP = mkfifo("wakawaka", 0644);
    printf("WKP created\n");

    // open
    int receiver = open("wakawaka", O_RDONLY);

    // read
    char message[HANDSHAKE_BUFFER_SIZE];
    int r = read(receiver, message, HANDSHAKE_BUFFER_SIZE);
    printf("client msg 0: %s\n", message);

    remove("main");
    printf("removed wkp\n");

    // write
    *to_client = open(message, O_WRONLY);
    int w = write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);
    r = read(receiver, message, HANDSHAKE_BUFFER_SIZE);
    printf("client msg 1: %s\n", message);
    
    return receiver;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    char pipe[HANDSHAKE_BUFFER_SIZE];
    sprintf(pipe, "%d", getpid());
    
    int pWKP = mkfifo(pipe, 0644);

    *to_server = open("wakawaka", O_WRONLY);
    if (*to_server == -1) {
        printf("error %d %s\n", errno, strerror(errno));
        remove(pipe);
        return 1;
    }

    // write
    int w = write(*to_server, pipe, HANDSHAKE_BUFFER_SIZE);

    int receiver = open(pipe, O_RDONLY);

    // receive
    char message[HANDSHAKE_BUFFER_SIZE];
    int r = read(receiver, message, HANDSHAKE_BUFFER_SIZE);
    printf("server msg: %s\n", message);

    // remove pipe
    remove(pipe);
    printf("removed pipe\n");

    w = write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
    
    return receiver;
}
