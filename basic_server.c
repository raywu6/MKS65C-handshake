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
        int wkp = mkfifo ("wakawaka", 0644);

        int wkp_read = open("wakawaka", O_RDONLY);
        char message[HANDSHAKE_BUFFER_SIZE];
        read(wkp_read, message, HANDSHAKE_BUFFER_SIZE);

        printf("%s\n", message); //server side

        // forking server
        int f = fork();

        if (f) {
            remove("wakawaka");
            close(wkp_read); //close read end of pipe
            printf("closed read end of pipe\n");
        }

        else {
            from_client = wkp_read;
            server_handshake( from_client, &to_client, message );
            
            
            char data[BUFFER_SIZE];
            while( read(from_client, data, BUFFER_SIZE) ) {
                process(data);
                printf("%s", data); //show msg on server view
                write(to_client, data, BUFFER_SIZE);
            }

            // close subserver
            close(to_client);
            close(from_client);
        }
    }
    
    // interrupted
    close(to_client);
    close(from_client);

    return 0;
}
