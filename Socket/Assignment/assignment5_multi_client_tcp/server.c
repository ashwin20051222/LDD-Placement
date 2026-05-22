#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 5005
#define BUFFER_SIZE 1024

typedef struct {
    int client_fd;
    struct sockaddr_in client_addr;
    int client_number;
} client_info_t;

static void print_timestamp(void) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char text[64];

    if (local_time != NULL && strftime(text, sizeof(text), "%Y-%m-%d %H:%M:%S", local_time) > 0) {
        printf("Server Timestamp: %s\n", text);
    }
}

static void *handle_client(void *arg) {
    client_info_t *info = (client_info_t *)arg;
    char buffer[BUFFER_SIZE];

    printf("Client %d Connected: %s:%d\n",
           info->client_number,
           inet_ntoa(info->client_addr.sin_addr),
           ntohs(info->client_addr.sin_port));
    print_timestamp();

    ssize_t bytes_read = recv(info->client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("Client %d: %s\n", info->client_number, buffer);
    } else if (bytes_read == 0) {
        printf("Client %d disconnected without sending data.\n", info->client_number);
    } else {
        perror("recv");
    }

    close(info->client_fd);
    free(info);
    return NULL;
}

int main(void) {
    int server_fd;
    int client_count = 0;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Multi-client TCP server listening on port %d...\n", PORT);

    while (1) {
        client_info_t *info = malloc(sizeof(*info));
        if (info == NULL) {
            perror("malloc");
            continue;
        }

        socklen_t client_len = sizeof(info->client_addr);
        info->client_fd = accept(server_fd, (struct sockaddr *)&info->client_addr, &client_len);
        if (info->client_fd < 0) {
            perror("accept");
            free(info);
            continue;
        }

        info->client_number = ++client_count;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, info) != 0) {
            perror("pthread_create");
            close(info->client_fd);
            free(info);
            continue;
        }

        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
