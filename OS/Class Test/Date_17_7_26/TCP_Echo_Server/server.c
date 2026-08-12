#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main() {
    int listen_fd, conn_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char buffer[BUF_SIZE];
    int port;

    printf("Enter port to listen on: ");
    if (scanf("%d", &port) != 1) {
        fprintf(stderr, "Invalid port\n");
        return 1;
    }

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); return 1; }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); close(listen_fd); return 1;
    }

    if (listen(listen_fd, 5) < 0) { perror("listen"); close(listen_fd); return 1; }

    printf("Waiting for connection on port %d...\n", port);
    conn_fd = accept(listen_fd, (struct sockaddr*)&cli_addr, &cli_len);
    if (conn_fd < 0) { perror("accept"); close(listen_fd); return 1; }

    printf("Client connected: %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    while (1) {
        ssize_t r = recv(conn_fd, buffer, BUF_SIZE-1, 0);
        if (r <= 0) break;
        buffer[r] = '\0';
        printf("Received: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) {
            send(conn_fd, "exit", 4, 0);
            break;
        }
        send(conn_fd, buffer, r, 0);
    }

    printf("Closing connection.\n");
    close(conn_fd);
    close(listen_fd);
    return 0;
}
