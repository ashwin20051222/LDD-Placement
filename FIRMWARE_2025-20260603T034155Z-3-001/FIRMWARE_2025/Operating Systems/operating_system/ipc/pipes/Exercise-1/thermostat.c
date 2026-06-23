/*
 * thermostat_control.c
 * Demonstration of IPC using pipe()
 * Read Process -> Control Process
 */
/*
Think of a thermostat like a temperature watchman.

You set a temperature (for example, 25°C).

It continuously checks the room temperature.

If the room gets hotter than 25°C → it turns ON the cooling (AC).

If the room gets colder than 25°C → it turns ON the heating.

When the desired temperature is reached → it turns the system OFF.

So basically, it keeps the temperature stable automatically.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SET_TEMPERATURE 25.0   // Desired temperature

int main() {
    int pipefd[2];
    pid_t pid;
    float sensor_value; // input sensor value
    float temperature;  // temperature

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }
    //create a child process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // 🔹 CHILD PROCESS → Read Process
    else if (pid == 0) {
        close(pipefd[0]);  // Close unused read end

        printf("Enter thermostat sensor value (in volts): ");
        scanf("%f", &sensor_value);

        // Example conversion formula
        temperature = sensor_value * 10;  

        printf("Read Process: Calculated Temperature = %.2f°C\n", temperature);

        write(pipefd[1], &temperature, sizeof(temperature));

        close(pipefd[1]);
        exit(0);
    }

    // 🔹 PARENT PROCESS → Control Process
    else {
        close(pipefd[1]);  // Close unused write end

        read(pipefd[0], &temperature, sizeof(temperature));

        printf("Control Process: Received Temperature = %.2f°C\n", temperature);

        if (temperature > SET_TEMPERATURE) {
            printf("Control Process: Compressor ON\n");
        } else {
            printf("Control Process: Compressor OFF\n");
        }

        close(pipefd[0]);
        wait(NULL);
    }

    return 0;
}

