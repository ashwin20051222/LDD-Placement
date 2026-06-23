/*
PROGRAM: Message Queues for IPC
QUESTION: Create and use message queues for communication between processes. 
          Demonstrate sending and receiving messages.
COMPILE: gcc -o msg_queue_demo msg_queue_demo.c
RUN: ./msg_queue_demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

struct MessageBuffer
{
    long messageType;
    char messageText[256];
};

int main()
{
    key_t msgKey;
    int msgID;
    struct MessageBuffer message;
    pid_t childPID;
    int i;
    
    printf("Message Queues - Inter-Process Communication\n");
    printf("===========================================\n\n");
    
    /* Generate unique key */
    msgKey = ftok(".", 'M');
    
    if (msgKey == -1)
    {
        perror("ftok");
        return 1;
    }
    
    printf("Message queue key: %d\n", msgKey);
    
    /* Create message queue */
    msgID = msgget(msgKey, 0666 | IPC_CREAT);
    
    if (msgID == -1)
    {
        perror("msgget");
        return 1;
    }
    
    printf("Message queue ID: %d\n", msgID);
    printf("Message queue created successfully\n\n");
    
    /* Create child process */
    childPID = fork();
    
    if (childPID == -1)
    {
        perror("fork");
        return 1;
    }
    
    if (childPID == 0)
    {
        /* Child process - sender */
        printf("[CHILD] Child process (PID: %d) - Message Sender\n", getpid());
        printf("[CHILD] Sending 3 messages to queue...\n\n");
        
        for (i = 1; i <= 3; i++)
        {
            message.messageType = i;
            snprintf(message.messageText, sizeof(message.messageText),
                    "Message %d from child process", i);
            
            printf("[CHILD] Sending message type %ld: %s\n", 
                   message.messageType, message.messageText);
            
            if (msgsnd(msgID, &message, strlen(message.messageText) + 1, 0) == -1)
            {
                perror("msgsnd");
                exit(1);
            }
            
            sleep(1);
        }
        
        printf("[CHILD] All messages sent\n");
        exit(0);
    }
    else
    {
        /* Parent process - receiver */
        printf("[PARENT] Parent process (PID: %d) - Message Receiver\n", getpid());
        printf("[PARENT] Waiting for messages...\n\n");
        
        sleep(2);  /* Let child send first message */
        
        for (i = 1; i <= 3; i++)
        {
            printf("[PARENT] Receiving message type %d...\n", i);
            
            if (msgrcv(msgID, &message, 256, i, 0) == -1)
            {
                perror("msgrcv");
                exit(1);
            }
            
            printf("[PARENT] Received message: %s\n\n", message.messageText);
        }
        
        printf("[PARENT] All messages received\n");
        
        /* Wait for child */
        wait(NULL);
        
        /* Delete message queue */
        if (msgctl(msgID, IPC_RMID, NULL) == -1)
        {
            perror("msgctl");
            return 1;
        }
        
        printf("[PARENT] Message queue deleted\n");
    }
    
    return 0;
}

/*
OUTPUT:
Message Queues - Inter-Process Communication
===========================================

Message queue key: 1234
Message queue ID: 0
Message queue created successfully

[PARENT] Parent process (PID: 12340) - Message Receiver
[PARENT] Waiting for messages...

[CHILD] Child process (PID: 12341) - Message Sender
[CHILD] Sending 3 messages to queue...

[CHILD] Sending message type 1: Message 1 from child process
[PARENT] Receiving message type 1...
[PARENT] Received message: Message 1 from child process

[CHILD] Sending message type 2: Message 2 from child process
[PARENT] Receiving message type 2...
[PARENT] Received message: Message 2 from child process

[CHILD] Sending message type 3: Message 3 from child process
[PARENT] Receiving message type 3...
[PARENT] Received message: Message 3 from child process

[CHILD] All messages sent
[PARENT] All messages received
[PARENT] Message queue deleted
*/
