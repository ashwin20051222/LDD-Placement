#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5002
#define BUFFER_SIZE 1024

static void trim_newline(char *text) {
    text[strcspn(text, "\n")] = '\0';
}

int main(void) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];
    char reply[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return 1;
    }

    while (1) {
        printf("[Client]: ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            break;
        }
        trim_newline(message);

        if (send(sock_fd, message, strlen(message), 0) < 0) {
            perror("send");
            break;
        }

        if (strcmp(message, "exit") == 0) {
            break;
        }

        ssize_t bytes_read = recv(sock_fd, reply, sizeof(reply) - 1, 0);
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        reply[bytes_read] = '\0';
        printf("[Server]: %s\n", reply);

        if (strcmp(reply, "exit") == 0) {
            break;
        }
    }

    close(sock_fd);
    return 0;
}
