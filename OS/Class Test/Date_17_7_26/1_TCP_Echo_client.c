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
    int sockfd;

    struct sockaddr_in server_addr;

    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Socket Created.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sockfd,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to Server.\n");

    while (1)
    {
        printf("\nEnter Message : ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        send(sockfd,
             buffer,
             strlen(buffer) + 1,
             0);

        recv(sockfd,
             buffer,
             sizeof(buffer),
             0);

        printf("Echo From Server : %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    close(sockfd);

    printf("Client Closed.\n");

    return 0;
}
