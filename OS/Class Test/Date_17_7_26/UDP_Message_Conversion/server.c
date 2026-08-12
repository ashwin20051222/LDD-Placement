#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char buffer[BUF_SIZE];
    int port;

    printf("Enter UDP port to listen on: ");
    if (scanf("%d", &port) != 1) { fprintf(stderr, "Invalid port\n"); return 1; }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) { perror("bind"); close(sockfd); return 1; }

    printf("UDP server listening on %d\n", port);
    // consume newline left by scanf
    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    while (1) {
        ssize_t r = recvfrom(sockfd, buffer, BUF_SIZE-1, 0, (struct sockaddr*)&cli_addr, &cli_len);
        if (r < 0) break;
        buffer[r] = '\0';
        printf("Received from %s:%d -> %s\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);

        if (strcmp(buffer, "exit") == 0) {
            sendto(sockfd, "exit", 4, 0, (struct sockaddr*)&cli_addr, cli_len);
            break;
        }

        for (ssize_t i=0;i<r;i++) buffer[i] = toupper((unsigned char)buffer[i]);
        sendto(sockfd, buffer, r, 0, (struct sockaddr*)&cli_addr, cli_len);
    }

    close(sockfd);
    return 0;
}
