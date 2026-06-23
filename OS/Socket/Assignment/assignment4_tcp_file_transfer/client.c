#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5004
#define BUFFER_SIZE 1024
#define INPUT_FILE "input.txt"

int main(void) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    FILE *file = fopen(INPUT_FILE, "rb");
    if (file == NULL) {
        perror("fopen input.txt");
        return 1;
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        fclose(file);
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        fclose(file);
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        fclose(file);
        close(sock_fd);
        return 1;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(sock_fd, buffer, bytes_read, 0) < 0) {
            perror("send");
            fclose(file);
            close(sock_fd);
            return 1;
        }
    }

    if (ferror(file)) {
        perror("fread");
        fclose(file);
        close(sock_fd);
        return 1;
    }

    printf("File Transfer Successful\n");

    fclose(file);
    close(sock_fd);
    return 0;
}
