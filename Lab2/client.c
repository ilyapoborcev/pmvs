#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        puts("usage: client <server ip> <command>");
        return 1;
    }

    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Error: Could not create socket!");
        return 2;
    }

    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(5042);

    if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) == -1) {
        puts("Error: Could not connect to server!");
        close(socket_desc);
        return 3;
    }

    if (send(socket_desc, argv[2], strlen(argv[2]), 0) == -1) {
        puts("Error: Could not send the command!");
        close(socket_desc);
        return 4;   
    }

    int bytes_count;
    char buf[20];
    while ((bytes_count = recv(socket_desc, buf, sizeof(buf)/sizeof(buf[0]), 0)) > 0) {
        fwrite(buf, bytes_count, 1, stdout);
    }

    close(socket_desc);

    return 0;
}
