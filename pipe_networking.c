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
    char message[BUFFER_SIZE];
    int r = read(receiver, message, BUFFER_SIZE);
    printf("client msg 0: %s\n", msg);

    remove("main");
    printf("removed wkp\n");

    // write
    *to_client = open(message, BUFFER_SIZE);
    int w = write(*to_client, message, BUFFER_SIZE);
    r = read(receiver, message, BUFFER_SIZE);
    printf("client msg 1: %s\n", msg);
    
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
    char pipe[BUFFER_SIZE];
    char buf[100];
    pipe = itoa(getpid(), buf, 10);

    int pWKP = mkfifo(pipe, 0644);

    *to_server = open("wakawaka", O_WRONLY);
    if (*to_server == -1) {
        printf("error %d %s\n", errno, strerror(errno));
    }

    // write
    int write = write(*to_server, pipe, BUFFER_SIZE);

    int receiver = open(pipe, O_RDONLY);

    // receive
    char message[BUFFER_SIZE];
    int read = read(receiver, message, BUFFER_SIZE);
    printf("server msg: %s\n", message);

    // remove pipe
    remove(pipe_name);
    printf("removed pipe\n");

    write = write(*to_server, message, BUFFER_SIZE);
    
    return receiver;
}
