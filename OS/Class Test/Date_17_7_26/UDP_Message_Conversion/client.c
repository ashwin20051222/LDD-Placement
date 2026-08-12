#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char server_ip[64];
    int port;
    char buffer[BUF_SIZE];
    socklen_t serv_len = sizeof(serv_addr);

    printf("Enter server IP: ");
    if (scanf("%63s", server_ip) != 1) return 1;
    printf("Enter server port: ");
    if (scanf("%d", &port) != 1) return 1;
    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) { perror("inet_pton"); close(sockfd); return 1; }

    printf("Type messages to send. Type 'exit' to quit.\n");
    while (1) {
        printf("Message: ");
        if (!fgets(buffer, BUF_SIZE, stdin)) break;
        buffer[strcspn(buffer, "\n")] = '\0';
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, serv_len) < 0) { perror("sendto"); break; }

        ssize_t r = recvfrom(sockfd, buffer, BUF_SIZE-1, 0, NULL, NULL);
        if (r <= 0) { printf("No response\n"); continue; }
        buffer[r] = '\0';
        printf("Converted: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;
    }

    close(sockfd);
    return 0;
}
