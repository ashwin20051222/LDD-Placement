#include <arpa/inet.h>
#include <ctype.h>
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
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sock_fd);
        return 1;
    }

    printf("UDP server listening on port %d...\n", PORT);

    ssize_t bytes_read = recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0,
                                  (struct sockaddr *)&client_addr, &client_len);
    if (bytes_read < 0) {
        perror("recvfrom");
        close(sock_fd);
        return 1;
    }

    buffer[bytes_read] = '\0';
    printf("Received from %s:%d: %s\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port),
           buffer);

    for (ssize_t i = 0; i < bytes_read; i++) {
        buffer[i] = (char)toupper((unsigned char)buffer[i]);
    }

    if (sendto(sock_fd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, client_len) < 0) {
        perror("sendto");
        close(sock_fd);
        return 1;
    }

    close(sock_fd);
    return 0;
}
