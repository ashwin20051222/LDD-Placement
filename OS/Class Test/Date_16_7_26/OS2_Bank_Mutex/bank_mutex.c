#include <stdio.h>
#include <pthread.h>

long balance = 10000;
pthread_mutex_t lock;

void *cashier(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&lock);
        if ((i + id) % 2 == 0) {
            balance += 50;
        } else {
            balance -= 30;
        }
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t threads[2];
    int ids[2] = {1, 2};

    pthread_mutex_init(&lock, NULL);

    pthread_create(&threads[0], NULL, cashier, &ids[0]);
    pthread_create(&threads[1], NULL, cashier, &ids[1]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_mutex_destroy(&lock);

    printf("Final account balance: %ld\n", balance);
    return 0;
}
