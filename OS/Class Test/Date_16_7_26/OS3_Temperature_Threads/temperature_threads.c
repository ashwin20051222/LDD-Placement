#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct SensorData {
    char name[20];
} node;

void *readSensor(void *arg) {
    node *sensor = (node *)arg;

    for (int i = 0; i < 5; i++) {
        int temp = 20 + rand() % 15;
        printf("Thread %lu - %s reading %d: %d C\n",
               (unsigned long)pthread_self(), sensor->name, i + 1, temp);
        sleep(1);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];
    node sensors[3] = {
        {"Sensor A"},
        {"Sensor B"},
        {"Sensor C"}
    };

    srand((unsigned int)time(NULL));

    pthread_create(&threads[0], NULL, readSensor, &sensors[0]);
    pthread_create(&threads[1], NULL, readSensor, &sensors[1]);
    pthread_create(&threads[2], NULL, readSensor, &sensors[2]);

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
