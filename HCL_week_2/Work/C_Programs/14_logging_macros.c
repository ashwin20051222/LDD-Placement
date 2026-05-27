/*
PROGRAM: Logging Macros with File, Line, and Function Information
QUESTION: Instrument code with logging macros (LOG_DEBUG, LOG_INFO, LOG_ERR) 
          that include file, line, and function information.
*/

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

/* Logging macros */
#define LOG_DEBUG(fmt, ...) \
    logMessage("[DEBUG]", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    logMessage("[INFO]", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    logMessage("[ERROR]", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

/* Get current time as string */
void getCurrentTime(char *timeStr)
{
    time_t now = time(NULL);
    struct tm *timeInfo = localtime(&now);
    strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", timeInfo);
}

/* Core logging function */
void logMessage(const char *level, const char *file, int line, 
                const char *function, const char *fmt, ...)
{
    char timeStr[20];
    getCurrentTime(timeStr);
    
    printf("[%s] %s | %s | %s:%d in %s() | ", 
           timeStr, level, level, file, line, function);
    
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf("\n");
}

/* Sample functions for demonstration */
void processUserData(int userId)
{
    LOG_DEBUG("Starting to process user data");
    LOG_INFO("Processing user ID: %d", userId);
    
    if (userId < 0)
    {
        LOG_ERROR("Invalid user ID: %d", userId);
        return;
    }
    
    LOG_INFO("User %d data processed successfully", userId);
    LOG_DEBUG("Exiting processUserData");
}

void authenticateUser(const char *username, const char *password)
{
    LOG_DEBUG("Authentication attempt for user: %s", username);
    
    if (username == NULL || password == NULL)
    {
        LOG_ERROR("NULL username or password provided");
        return;
    }
    
    if (strlen(password) < 6)
    {
        LOG_ERROR("Password too short for user: %s", username);
        return;
    }
    
    LOG_INFO("User %s authenticated successfully", username);
}

void allocateMemory(int size)
{
    LOG_DEBUG("Requesting memory allocation of %d bytes", size);
    
    if (size <= 0)
    {
        LOG_ERROR("Invalid memory size requested: %d", size);
        return;
    }
    
    if (size > 1000000)
    {
        LOG_ERROR("Memory request exceeds limit: %d bytes", size);
        return;
    }
    
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        LOG_ERROR("Memory allocation failed for %d bytes", size);
    }
    else
    {
        LOG_INFO("Memory allocated successfully: %p", ptr);
        free(ptr);
        LOG_DEBUG("Memory freed");
    }
}

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    printf("Logging System with Macros\n");
    printf("===========================\n\n");
    
    printf("--- Test 1: Process User Data ---\n");
    processUserData(101);
    printf("\n");
    
    printf("--- Test 2: Invalid User ID ---\n");
    processUserData(-5);
    printf("\n");
    
    printf("--- Test 3: Authenticate User ---\n");
    authenticateUser("john_doe", "password123");
    printf("\n");
    
    printf("--- Test 4: Weak Password ---\n");
    authenticateUser("jane_doe", "pass");
    printf("\n");
    
    printf("--- Test 5: Null Username ---\n");
    authenticateUser(NULL, "password123");
    printf("\n");
    
    printf("--- Test 6: Memory Allocation ---\n");
    allocateMemory(512);
    printf("\n");
    
    printf("--- Test 7: Invalid Memory Size ---\n");
    allocateMemory(-100);
    printf("\n");
    
    printf("--- Test 8: Exceeded Memory Limit ---\n");
    allocateMemory(5000000);
    
    return 0;
}

/*
OUTPUT:
Logging System with Macros
===========================

--- Test 1: Process User Data ---
[2026-05-27 09:39:38] [DEBUG] [DEBUG] 13_logging_macros.c:72 in processUserData() | Starting to process user data
[2026-05-27 09:39:38] [INFO] [INFO] 13_logging_macros.c:73 in processUserData() | Processing user ID: 101
[2026-05-27 09:39:38] [INFO] [INFO] 13_logging_macros.c:80 in processUserData() | User 101 data processed successfully
[2026-05-27 09:39:38] [DEBUG] [DEBUG] 13_logging_macros.c:81 in processUserData() | Exiting processUserData

--- Test 2: Invalid User ID ---
[2026-05-27 09:39:38] [DEBUG] [DEBUG] 13_logging_macros.c:72 in processUserData() | Starting to process user data
[2026-05-27 09:39:38] [INFO] [INFO] 13_logging_macros.c:73 in processUserData() | Processing user ID: -5
[2026-05-27 09:39:38] [ERROR] [ERROR] 13_logging_macros.c:77 in processUserData() | Invalid user ID: -5

--- Test 3: Authenticate User ---
[2026-05-27 09:39:38] [DEBUG] [DEBUG] 13_logging_macros.c:84 in authenticateUser() | Authentication attempt for user: john_doe
[2026-05-27 09:39:38] [INFO] [INFO] 13_logging_macros.c:93 in authenticateUser() | User john_doe authenticated successfully

... (more output)
*/
