#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd;
    int client_fd;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    socklen_t client_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Bind successful.\n");

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for client...\n");

    client_fd = accept(server_fd,
                      (struct sockaddr *)&client_addr,
                      &client_len);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client Connected.\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(client_fd,
                         buffer,
                         sizeof(buffer),
                         0);

        if (bytes <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client : %s\n", buffer);

        send(client_fd,
             buffer,
             strlen(buffer) + 1,
             0);

        if (strcmp(buffer, "exit") == 0)
        {
            printf("Exit received.\n");
            break;
        }
    }

    close(client_fd);
    close(server_fd);

    printf("Server Closed.\n");

    return 0;
}
