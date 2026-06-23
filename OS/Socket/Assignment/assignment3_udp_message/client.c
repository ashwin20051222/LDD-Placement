#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5003
#define BUFFER_SIZE 1024

int main(void) {
    int sock_fd;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE];
    const char *message = "Networking Lab";

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
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

    if (sendto(sock_fd, message, strlen(message), 0,
               (struct sockaddr *)&server_addr, server_len) < 0) {
        perror("sendto");
        close(sock_fd);
        return 1;
    }

    printf("Sent: %s\n", message);

    ssize_t bytes_read = recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0,
                                  (struct sockaddr *)&server_addr, &server_len);
    if (bytes_read < 0) {
        perror("recvfrom");
        close(sock_fd);
        return 1;
    }

    buffer[bytes_read] = '\0';
    printf("Received: %s\n", buffer);

    close(sock_fd);
    return 0;
}
