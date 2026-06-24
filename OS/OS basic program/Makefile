# Common Makefile for OS Basic Programs
CC = gcc
CFLAGS = -Wall -Wextra
PTHREAD_FLAGS = -pthread
IPC_LIBS = 

# Program directories and targets
PROGRAM_1 = 1_File_Handling/1_file_handling
PROGRAM_2 = 2_Process/2_process_id
PROGRAM_3 = 3_Child_Process/3_child_process
PROGRAM_4 = 4_Threads/4_threads
PROGRAM_5 = 5_Mutex/5_mutex
PROGRAM_6 = 6_Semaphore/6_semaphore
PROGRAM_7 = 7_Unnamed_Pipe/7_unnamed_pipe
PROGRAM_8 = 8_Named_Pipe/8_named_pipe
PROGRAM_9 = 9_Shared_Memory/9_shared_memory
PROGRAM_10 = 10_Message_Queue/10_message_queue
PROGRAM_11 = 11_TCP_Socket/11_tcp_socket
PROGRAM_12 = 12_UDP_Socket/12_udp_socket

# All programs
ALL_PROGRAMS = $(PROGRAM_1) $(PROGRAM_2) $(PROGRAM_3) $(PROGRAM_4) $(PROGRAM_5) $(PROGRAM_6) $(PROGRAM_7) $(PROGRAM_8) $(PROGRAM_9) $(PROGRAM_10) $(PROGRAM_11) $(PROGRAM_12)

# Default target
all: $(ALL_PROGRAMS)

# Compile simple programs (1, 2, 3)
$(PROGRAM_1): 1_File_Handling/1_file_handling.c
	$(CC) $(CFLAGS) -o $@ $<

$(PROGRAM_2): 2_Process/2_process_id.c
	$(CC) $(CFLAGS) -o $@ $<

$(PROGRAM_3): 3_Child_Process/3_child_process.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile thread programs (4, 5, 6)
$(PROGRAM_4): 4_Threads/4_threads.c
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) -o $@ $<

$(PROGRAM_5): 5_Mutex/5_mutex.c
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) -o $@ $<

$(PROGRAM_6): 6_Semaphore/6_semaphore.c
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) -o $@ $<

# Compile pipe programs (7, 8)
$(PROGRAM_7): 7_Unnamed_Pipe/7_unnamed_pipe.c
	$(CC) $(CFLAGS) -o $@ $<

$(PROGRAM_8): 8_Named_Pipe/8_named_pipe.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile IPC programs (9, 10)
$(PROGRAM_9): 9_Shared_Memory/9_shared_memory.c
	$(CC) $(CFLAGS) -o $@ $<

$(PROGRAM_10): 10_Message_Queue/10_message_queue.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile socket programs (11, 12)
$(PROGRAM_11): 11_TCP_Socket/11_tcp_socket.c
	$(CC) $(CFLAGS) -o $@ $<

$(PROGRAM_12): 12_UDP_Socket/12_udp_socket.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean all compiled binaries
clean:
	rm -f $(ALL_PROGRAMS)

# Phony targets
.PHONY: all clean

# Help target
help:
	@echo "Available targets:"
	@echo "  make all        - Compile all programs"
	@echo "  make clean      - Remove all compiled binaries"
	@echo "  make help       - Display this help message"
	@echo ""
	@echo "Individual programs:"
	@echo "  make $(PROGRAM_1)"
	@echo "  make $(PROGRAM_2)"
	@echo "  make $(PROGRAM_3)"
	@echo "  make $(PROGRAM_4)"
	@echo "  make $(PROGRAM_5)"
	@echo "  make $(PROGRAM_6)"
	@echo "  make $(PROGRAM_7)"
	@echo "  make $(PROGRAM_8)"
	@echo "  make $(PROGRAM_9)"
	@echo "  make $(PROGRAM_10)"
	@echo "  make $(PROGRAM_11)"
	@echo "  make $(PROGRAM_12)"
