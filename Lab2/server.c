#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

void * process_request(void * arg) 
{
    long socket = (long) arg;
    char buf[1024] = { 0 };
    int bytes_count = recv(socket, buf, sizeof(buf)/sizeof(buf[0]), 0);
    FILE *pipe = popen(buf, "r");

    while (bytes_count = fread(buf, 1, sizeof(buf)/sizeof(buf[0]), pipe)) {
        write(socket, buf, bytes_count);
    }

    pclose(pipe);

    close(socket);
}

int main(int argc, char **argv)
{
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Error: Could not create socket!");
        return 2;
    }

    int enable = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    struct sockaddr_in server;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(5042);

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) == -1) {
        puts("Error: Could not bind the socket!");
        close(socket_desc);
        return 3;    
    }

	 puts("Socket created!");

    listen(socket_desc, 5);

    struct sockaddr_in client;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int new_socket;
    int bytes_count;
    char buf[1024];
    FILE *pipe;
    pthread_t thread;
    while (1) {
        memset(buf, 0, sizeof(buf)/sizeof(buf[0]));

        if ((new_socket = accept(socket_desc, (struct sockaddr *) &client, &socklen)) == -1) {
            continue;
        }

#if PROCESS        
        pid_t pid = fork();
        if (pid == -1) {
            puts("Error: Could not create a process!");
            return 4;
        }
        if (!pid) {
            process_request((void*)(long) new_socket);
            _exit(0);
        } else {
            close(new_socket);
        }
#else
        if (pthread_create(&thread, NULL, process_request, (void *)(long) new_socket)) {
            puts("Error: Could not create a thread!");
            return 4;
        }
#endif
    }

    close(socket_desc);

    return 0;
}
