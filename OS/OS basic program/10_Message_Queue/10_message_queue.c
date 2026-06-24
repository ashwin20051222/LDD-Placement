#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/ipc.h"
#include "sys/msg.h"

struct message {
	long msg_type;
	char msg_text[100];
};

int main() {
	key_t key = ftok("/tmp", 'Q');
	
	if(key == -1) {
		perror("ftok failed");
		return 1;
	}
	
	int msgid = msgget(key, IPC_CREAT | 0666);
	
	if(msgid == -1) {
		perror("msgget failed");
		return 1;
	}
	
	struct message msg;
	msg.msg_type = 1;
	strcpy(msg.msg_text, "Hello Queue");
	
	// Send the message
	if(msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
		perror("msgsnd failed");
		return 1;
	}
	
	printf("Message sent to queue: %s\n", msg.msg_text);
	
	// Receive the message
	struct message received_msg;
	if(msgrcv(msgid, &received_msg, sizeof(received_msg.msg_text), 1, 0) == -1) {
		perror("msgrcv failed");
		return 1;
	}
	
	printf("Message received from queue: %s\n", received_msg.msg_text);
	
	// Remove the message queue
	msgctl(msgid, IPC_RMID, NULL);
	
	return 0;
}
