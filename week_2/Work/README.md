# C Programming - Complete Tutorial & Examples

## Overview

This directory contains **35 beginner-friendly C programs** organized into two folders:
- **C_Programs/**: Core algorithms, data structures, and C fundamentals
- **Linux_Programs/**: Linux-specific system programming with IPC and process management

All programs are written in **humanized, beginner-friendly style** with:
- Detailed comments explaining logic
- Question and output included in code comments
- No complex syntax or unnecessary optimizations
- Practical examples for learning

---

## C_Programs Folder (25 Programs)

### Basic Algorithms & Search
1. **01_linear_search.c** - Find element position in array using linear search
   - Concepts: Array traversal, conditional logic
   - Time Complexity: O(n)

2. **02_quicksort.c** - Sort array using QuickSort algorithm
   - Concepts: Recursion, divide-and-conquer, array partitioning
   - Time Complexity: O(n log n) average

### Stack Operations
3. **03_balanced_brackets_stack.c** - Check balanced parentheses/brackets
   - Concepts: Stack LIFO, string parsing, bracket matching
   - Use Cases: Expression validation, compiler design

4. **14_logging_macros.c** - Implement logging system with macros
   - Concepts: Preprocessor macros, variadic functions, debugging
   - Use Cases: Application logging, debugging framework

### Queue Operations
5. **04_circular_queue_array.c** - Implement circular queue with ring buffer
   - Concepts: Circular arrays, wrap-around, modulo arithmetic
   - Operations: Enqueue, dequeue, display

6. **05_queue_two_stacks.c** - Build queue using two stacks
   - Concepts: Stack composition, amortized complexity O(1)
   - Analysis: Transfer optimization technique

### Binary Search Trees (BST)
7. **06_binary_search_tree.c** - BST with insert, search, traversal
   - Concepts: Tree structure, recursion, inorder traversal
   - Operations: Insert, search, preorder, inorder

8. **07_bst_delete.c** - Delete node from BST (all 3 cases)
   - Concepts: Handling leaf, one-child, two-child nodes
   - Use Cases: Maintaining dynamic sorted data

### Linked Lists
9. **08_circular_linked_list_music.c** - Music player with circular list
   - Concepts: Circular structure, continuous traversal
   - Real-world: Playlist implementation with 5+ songs

10. **09_doubly_linked_list_railway.c** - Railway compartment management
    - Concepts: Bidirectional traversal, insertion/deletion
    - Real-world: 4+ compartment management system

11. **22_merge_sorted_lists.c** - Merge two sorted linked lists
    - Concepts: List merging, pointer manipulation
    - Time Complexity: O(n + m)

### Tree Analysis
12. **10_tree_height_balance.c** - Calculate height & check balance
    - Concepts: Tree traversal, height calculation
    - AVL tree concepts

### Mathematical Operations
13. **11_arithmetic_dynamic.c** - Dynamic number arithmetic operations
    - Concepts: Dynamic memory, array processing, calculations
    - Operations: Sum, average, product, min, max

### String Processing
14. **12_string_length.c** - String operations and analysis
    - Concepts: String manipulation, character analysis
    - Operations: Length, reverse, vowel/consonant count

15. **16_ctype_operations.c** - Character classification with ctype.h
    - Concepts: Character type checking, case conversion
    - Functions: isalpha, isdigit, toupper, tolower

16. **15_file_io_tokens.c** - Read file line-by-line and tokenize
    - Concepts: File I/O with fgets, string tokenization
    - Functions: fgets, strtok, trim whitespace

### Input Validation & Statistics
17. **13_input_parser.c** - Robust input parser with error codes
    - Concepts: Validation, error handling, range checking
    - Returns: Rich error messages and codes

18. **17_math_statistics.c** - Statistical calculations with math.h
    - Concepts: Mean, variance, standard deviation, Z-scores
    - Functions: sqrt, calculations on double arrays

### Advanced Array Problems
19. **18_sort_0_1_2.c** - Sort array with only 0s, 1s, 2s
    - Concepts: Counting sort variant, in-place sorting
    - Time Complexity: O(n), Space: O(1)

20. **19_rotate_array.c** - Rotate array right by k steps efficiently
    - Concepts: Array rotation, reverse technique
    - Algorithm: O(n) time using reversal

### Advanced Stack Problems
21. **20_balanced_brackets_list.c** - Balanced brackets with list-based stack
    - Concepts: Stack implementation, bracket validation
    - Multiple test cases with different patterns

22. **21_rpn_calculator.c** - Reverse Polish Notation calculator
    - Concepts: Expression evaluation, operator precedence
    - Examples: "2 1 +" evaluates to 3

### Editor Features
23. **23_undo_redo_stacks.c** - Text editor undo/redo with two stacks
    - Concepts: State management, dual stack pattern
    - Operations: Insert, delete, undo, redo

### Scheduling & Queues
24. **24_round_robin_scheduler.c** - CPU scheduler with round-robin
    - Concepts: Time quantum, process scheduling
    - Metrics: Context switches, completion time

25. **25_sliding_window_deque.c** - Find max in sliding window
    - Concepts: Deque implementation, sliding window
    - Time Complexity: O(n) using deque optimization

---

## Linux_Programs Folder (10 Programs)

### Memory Management
1. **01_memory_management.c** - Dynamic memory allocation
   - Functions: malloc(), calloc(), realloc(), free()
   - Concepts: Heap management, memory efficiency

### Inter-Process Communication (IPC)
2. **02_pipes.c** - Pipe-based communication between processes
   - Concepts: Anonymous pipes, parent-child communication
   - Read/write endpoints, file descriptors

3. **03_fork_processes.c** - Process creation and management
   - Concepts: fork(), parent/child processes, wait()
   - Status codes, process hierarchy

4. **04_shared_memory.c** - Shared memory for IPC
   - Functions: shmget(), shmat(), shmdt(), shmctl()
   - Concepts: Memory segments, shared data structures

5. **05_semaphores.c** - Process synchronization with semaphores
   - Functions: semget(), semop(), semctl()
   - Concepts: Critical sections, race condition prevention

6. **06_named_pipes_fifo.c** - FIFO for inter-process communication
   - Functions: mkfifo(), open(), read(), write()
   - Concepts: Named pipes, blocking I/O

7. **07_message_queues.c** - Message queue for IPC
   - Functions: msgget(), msgsnd(), msgrcv(), msgctl()
   - Concepts: Message types, queue operations

### Memory & File Operations
8. **08_memory_mapping.c** - File memory mapping with mmap()
   - Functions: mmap(), munmap(), msync()
   - Concepts: Virtual memory, efficient file I/O

### Signal Handling
9. **09_signal_handling.c** - Signal handlers (SIGINT, SIGTERM, SIGUSR1)
   - Functions: signal(), sigaction()
   - Concepts: Asynchronous event handling, cleanup

### Threading
10. **10_thread_creation.c** - Multi-threading with pthreads
    - Functions: pthread_create(), pthread_join()
    - Concepts: Thread lifecycle, data passing, synchronization

---

## How to Compile & Run

### Single Program
```bash
# For C_Programs (no special libraries)
gcc -o program_name C_Programs/XX_program_name.c
./program_name

# For Linux_Programs with pthread
gcc -o program_name Linux_Programs/10_thread_creation.c -lpthread
./program_name
```

### Compile All
```bash
# Compile C_Programs
cd C_Programs
for file in *.c; do gcc -o "${file%.c}" "$file"; done

# Compile Linux_Programs
cd ../Linux_Programs
for file in *.c; do
  if [[ "$file" == "10_thread"* ]]; then
    gcc -o "${file%.c}" "$file" -lpthread
  else
    gcc -o "${file%.c}" "$file"
  fi
done
```

---

## Learning Path

### Beginner (Start Here)
1. 01_linear_search.c - Basic array operations
2. 02_quicksort.c - Sorting fundamentals
3. 12_string_length.c - String basics
4. 11_arithmetic_dynamic.c - Dynamic memory intro

### Intermediate
5. 03_balanced_brackets_stack.c - Stack understanding
6. 04_circular_queue_array.c - Queue operations
7. 06_binary_search_tree.c - Tree structures
8. 08_circular_linked_list_music.c - Linked lists

### Advanced
9. 07_bst_delete.c - Complex tree operations
10. 21_rpn_calculator.c - Expression evaluation
11. 23_undo_redo_stacks.c - State management
12. 25_sliding_window_deque.c - Optimized algorithms

### System Programming
1. 01_memory_management.c - Memory fundamentals
2. 03_fork_processes.c - Process creation
3. 02_pipes.c - Basic IPC
4. 05_semaphores.c - Synchronization
5. 10_thread_creation.c - Concurrency

---

## Key Concepts by Topic

### Data Structures
- **Array**: Linear search, QuickSort, rotate, sort 0-1-2
- **Stack**: Bracket validation, RPN calculator, undo/redo
- **Queue**: Circular queue, queue from stacks, scheduler
- **Tree**: BST operations, height calculation, deletion
- **Linked List**: Circular, doubly, merging

### Algorithms
- **Sorting**: QuickSort
- **Searching**: Linear search, binary tree search
- **Math**: Statistics, arithmetic operations
- **String**: Tokenization, classification, validation

### System Programming
- **Memory**: malloc, calloc, realloc, free, mmap
- **IPC**: Pipes, shared memory, message queues, semaphores
- **Processes**: fork, wait, process management
- **Signals**: Signal handlers, cleanup
- **Threads**: Thread creation, joining, data passing

### Libraries Used
- **Standard**: stdio.h, stdlib.h, string.h, math.h
- **Character**: ctype.h (character classification)
- **File I/O**: fcntl.h, unistd.h
- **IPC**: sys/ipc.h, sys/shm.h, sys/msg.h, sys/sem.h
- **Threading**: pthread.h (requires -lpthread)

---

## Important Notes

### Code Style
- All code uses **beginner-friendly style**
- Minimal use of advanced C features
- Clear variable names and logical flow
- Comments explain the "why", not just the "what"

### Output Format
- Every program has expected output in comments
- Output format: `/* OUTPUT: ... */`
- Use this to verify correct execution

### Common Compilation Flags
```bash
# Basic compilation
gcc -o program program.c

# With warnings
gcc -Wall -Wextra -o program program.c

# With debugging
gcc -g -o program program.c

# With pthread
gcc -pthread -o program program.c

# With math library
gcc -o program program.c -lm
```

