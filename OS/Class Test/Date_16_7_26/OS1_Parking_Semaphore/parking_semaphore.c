#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TOTAL_CARS 10
#define PARKING_SLOTS 5

sem_t slots;

void *car(void *arg) {
    int carNumber = *(int *)arg;

    printf("Car %d is waiting.\n", carNumber);
    sem_wait(&slots);
    printf("Car %d is parked.\n", carNumber);
    sleep(1);
    printf("Car %d is leaving.\n", carNumber);
    sem_post(&slots);

    return NULL;
}

int main() {
    pthread_t threads[TOTAL_CARS];
    int carNumbers[TOTAL_CARS];

    sem_init(&slots, 0, PARKING_SLOTS);

    for (int i = 0; i < TOTAL_CARS; i++) {
        carNumbers[i] = i + 1;
        pthread_create(&threads[i], NULL, car, &carNumbers[i]);
    }

    for (int i = 0; i < TOTAL_CARS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&slots);
    return 0;
}
