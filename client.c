#include "pipe_networking.h"


int main() {

    int to_server;
    int from_server;

    from_server = client_handshake( &to_server );

    char data[BUFFER_SIZE];

    while (1) {
        printf("Message: ");
        fgets(data, BUFFER_SIZE, stdin);
        write(to_server, data, BUFFER_SIZE);
        read(from_server, data, BUFFER_SIZE);
        printf("%s\n", data); //client side message
    }
}
