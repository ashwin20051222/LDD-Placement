#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

#define PORT 8081
#define SENDER_IP "127.0.0.1"

int main() {
	int sock;
	struct sockaddr_in receiver_addr;
	socklen_t addr_len = sizeof(receiver_addr);
	char message[1024] = "Hello from UDP Sender";
	char buffer[1024] = {0};
	
	// Create UDP socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sock == -1) {
		perror("socket failed");
		return 1;
	}
	
	// Initialize receiver address structure
	memset(&receiver_addr, 0, sizeof(receiver_addr));
	receiver_addr.sin_family = AF_INET;
	receiver_addr.sin_port = htons(PORT);
	receiver_addr.sin_addr.s_addr = inet_addr(SENDER_IP);
	
	// Send message
	ssize_t sent = sendto(sock, message, strlen(message), 0, 
	                      (struct sockaddr*)&receiver_addr, addr_len);
	
	if(sent == -1) {
		perror("sendto failed");
		return 1;
	}
	
	printf("Message sent: %s\n", message);
	
	// Set socket to receive mode
	struct sockaddr_in local_addr;
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
		perror("bind failed");
		return 1;
	}
	
	printf("UDP Receiver listening on port %d...\n", PORT);
	
	// Receive message
	struct sockaddr_in sender_addr;
	socklen_t sender_addr_len = sizeof(sender_addr);
	
	ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0, 
	                            (struct sockaddr*)&sender_addr, &sender_addr_len);
	
	if(received > 0) {
		buffer[received] = '\0';
		printf("Message received: %s from %s:%d\n", buffer, 
		       inet_ntoa(sender_addr.sin_addr), ntohs(sender_addr.sin_port));
	}
	
	close(sock);
	
	return 0;
}
