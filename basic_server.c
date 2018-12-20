#include "pipe_networking.h"


static void sighandler(int signo) {
    if (signo == SIGINT) {
        remove("wakawaka");
        exit(0);
    }
}

void process(char *data) {
    int i = 0;
    while (data[i]) {
        if ( data[i] == 'a') {
            data[i] = '4';
        }
        else if ( data[i] == 'e') {
            data[i] = '3';
        }
        else if ( data[i] == 'i') {
            data[i] = '1';
        }
        else if ( data[i] == 'o') {
            data[i] = '0';
        }
        else if ( data[i] == 'u') {
            data[i] = 'v';
        }

        i++;
    }
}

int main() {

    int to_client;
    int from_client;

    signal(SIGINT, sighandler);
    
    // keep running until interrupted
    while(1) {
        from_client = server_handshake( &to_client );

        char data[BUFFER_SIZE];
        while (read(from_client, data, BUFFER_SIZE)) {
            process(data);
            printf("%s", data); //show msg on server view
            write(to_client, data, BUFFER_SIZE);
        }
    }
    // interrupted
    close(to_client);
    close(from_client);
    
}
