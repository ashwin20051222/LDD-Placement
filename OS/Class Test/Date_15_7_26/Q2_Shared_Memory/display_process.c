#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

typedef struct {
    int temperature;
    int active;
} shared_temperature;

int main(void)
{
    key_t key;
    int shmid;
    int continueChoice;
    int removeChoice;
    shared_temperature *data;

    printf("Enter shared memory key (integer) : ");
    scanf("%d", &key);

    shmid = shmget(key, sizeof(shared_temperature), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    data = (shared_temperature *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat failed");
        return 1;
    }

    while (1) {
        printf("Latest temperature: %d C\n", data->temperature);
        if (data->temperature > 40) {
            printf("ALERT: High temperature detected!\n");
        }
        if (data->active == 0) {
            printf("Sensor process marked monitoring as stopped\n");
        }

        sleep(1);
        printf("Read next value? (1-Yes, 0-No) : ");
        scanf("%d", &continueChoice);
        if (continueChoice == 0) {
            break;
        }
    }

    if (shmdt(data) == -1) {
        perror("shmdt failed");
        return 1;
    }

    printf("Remove shared memory segment now? (1-Yes, 0-No) : ");
    scanf("%d", &removeChoice);
    if (removeChoice == 1) {
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl IPC_RMID failed");
            return 1;
        }
        printf("Shared memory removed\n");
    }

    return 0;
}
