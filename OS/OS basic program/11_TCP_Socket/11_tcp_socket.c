#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#define PORT 8080
#define BACKLOG 1

int main() {
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	char buffer[1024] = {0};
	
	// Create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(server_fd == -1) {
		perror("socket failed");
		return 1;
	}
	
	// Initialize server address structure
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	
	// Bind socket to port
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind failed");
		return 1;
	}
	
	// Listen for incoming connections
	if(listen(server_fd, BACKLOG) == -1) {
		perror("listen failed");
		return 1;
	}
	
	printf("TCP Server listening on port %d...\n", PORT);
	
	// Accept incoming connection
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
	
	if(client_fd == -1) {
		perror("accept failed");
		return 1;
	}
	
	printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	
	// Receive message from client
	int n = recv(client_fd, buffer, sizeof(buffer), 0);
	
	if(n > 0) {
		printf("Message received: %s\n", buffer);
	}
	
	// Close connections
	close(client_fd);
	close(server_fd);
	
	return 0;
}
