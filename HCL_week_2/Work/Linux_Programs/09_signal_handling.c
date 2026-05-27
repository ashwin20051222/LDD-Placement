/*
PROGRAM: Signal Handling in Linux
QUESTION: Demonstrate signal handling with signal handlers for 
          SIGINT, SIGTERM, and custom signals.
COMPILE: gcc -o signal_demo signal_demo.c
RUN: ./signal_demo
INTERACT: Press Ctrl+C (SIGINT) or send signals from another terminal
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

/* Global counter */
int signalCount = 0;

/* Signal handler for SIGINT (Ctrl+C) */
void handleSIGINT(int sig)
{
    printf("\n[SIGNAL] Received SIGINT (signal %d) - Ctrl+C pressed\n", sig);
    signalCount++;
    printf("[SIGNAL] Signal count: %d\n", signalCount);
    
    if (signalCount >= 3)
    {
        printf("[SIGNAL] Received 3 signals. Exiting gracefully...\n");
        exit(0);
    }
}

/* Signal handler for SIGTERM */
void handleSIGTERM(int sig)
{
    printf("\n[SIGNAL] Received SIGTERM (signal %d) - Termination signal\n", sig);
    printf("[SIGNAL] Cleaning up resources...\n");
    exit(1);
}

/* Signal handler for SIGUSR1 */
void handleSIGUSR1(int sig)
{
    printf("\n[SIGNAL] Received SIGUSR1 (signal %d) - User defined signal\n", sig);
    printf("[SIGNAL] Custom action triggered\n");
}

int main()
{
    struct sigaction sa;
    
    printf("Signal Handling in Linux\n");
    printf("=======================\n\n");
    
    printf("Process ID: %d\n", getpid());
    printf("This process will handle signals: SIGINT, SIGTERM, SIGUSR1\n\n");
    
    /* Setup SIGINT handler */
    printf("--- Registering Signal Handlers ---\n");
    printf("SIGINT (Ctrl+C): Will increment counter (exit after 3 signals)\n");
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSIGINT;
    
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction SIGINT");
        return 1;
    }
    
    /* Setup SIGTERM handler */
    printf("SIGTERM: Will cleanup and exit\n");
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSIGTERM;
    
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction SIGTERM");
        return 1;
    }
    
    /* Setup SIGUSR1 handler */
    printf("SIGUSR1: Will print custom message\n");
    
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSIGUSR1;
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction SIGUSR1");
        return 1;
    }
    
    printf("\nSignal handlers registered successfully\n\n");
    
    printf("--- Instructions ---\n");
    printf("1. Press Ctrl+C (SIGINT) to increment counter\n");
    printf("2. Use 'kill -TERM %d' from another terminal for SIGTERM\n", getpid());
    printf("3. Use 'kill -USR1 %d' from another terminal for SIGUSR1\n\n", getpid());
    
    printf("--- Running Signal Handler Demo ---\n");
    
    /* Main loop */
    int count = 0;
    while (1)
    {
        printf("Main loop iteration %d (PID: %d)\n", ++count, getpid());
        sleep(2);
    }
    
    return 0;
}

/*
OUTPUT:
Signal Handling in Linux
=======================

Process ID: 12345
This process will handle signals: SIGINT, SIGTERM, SIGUSR1

--- Registering Signal Handlers ---
SIGINT (Ctrl+C): Will increment counter (exit after 3 signals)
SIGTERM: Will cleanup and exit
SIGUSR1: Will print custom message

Signal handlers registered successfully

--- Instructions ---
1. Press Ctrl+C (SIGINT) to increment counter
2. Use 'kill -TERM 12345' from another terminal for SIGTERM
3. Use 'kill -USR1 12345' from another terminal for SIGUSR1

--- Running Signal Handler Demo ---
Main loop iteration 1 (PID: 12345)
Main loop iteration 2 (PID: 12345)

[SIGNAL] Received SIGINT (signal 2) - Ctrl+C pressed
[SIGNAL] Signal count: 1
Main loop iteration 3 (PID: 12345)
Main loop iteration 4 (PID: 12345)

[SIGNAL] Received SIGUSR1 (signal 10) - User defined signal
[SIGNAL] Custom action triggered
Main loop iteration 5 (PID: 12345)

[SIGNAL] Received SIGINT (signal 2) - Ctrl+C pressed
[SIGNAL] Signal count: 2
Main loop iteration 6 (PID: 12345)

[SIGNAL] Received SIGINT (signal 2) - Ctrl+C pressed
[SIGNAL] Signal count: 3
[SIGNAL] Received 3 signals. Exiting gracefully...
*/
