#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int transactionId;
    int type;
    double amount;
} transaction;

double accountBalance = 0.0;
int successfulTransactions = 0;
pthread_mutex_t balanceMutex;

void *processTransaction(void *arg)
{
    transaction *tx = (transaction *)arg;

    pthread_mutex_lock(&balanceMutex);

    if (tx->type == 1) {
        accountBalance += tx->amount;
        successfulTransactions++;
        printf("Transaction %d: Deposit %.2f successful. Balance = %.2f\n",
               tx->transactionId, tx->amount, accountBalance);
    } else if (tx->type == 2) {
        if (accountBalance >= tx->amount) {
            accountBalance -= tx->amount;
            successfulTransactions++;
            printf("Transaction %d: Withdrawal %.2f successful. Balance = %.2f\n",
                   tx->transactionId, tx->amount, accountBalance);
        } else {
            printf("Transaction %d: Withdrawal %.2f failed. Insufficient balance = %.2f\n",
                   tx->transactionId, tx->amount, accountBalance);
        }
    } else if (tx->type == 3) {
        successfulTransactions++;
        printf("Transaction %d: Balance Inquiry. Current Balance = %.2f\n",
               tx->transactionId, accountBalance);
    } else {
        printf("Transaction %d: Invalid transaction type\n", tx->transactionId);
    }

    pthread_mutex_unlock(&balanceMutex);
    return NULL;
}

int main(void)
{
    int n;
    int i;
    pthread_t *threads;
    int *threadCreated;
    transaction *transactions;

    printf("Enter initial account balance : ");
    scanf("%lf", &accountBalance);

    printf("Enter number of transactions : ");
    scanf("%d", &n);

    threads = (pthread_t *)malloc(sizeof(pthread_t) * n);
    threadCreated = (int *)calloc((size_t)n, sizeof(int));
    transactions = (transaction *)malloc(sizeof(transaction) * n);
    if (threads == NULL || threadCreated == NULL || transactions == NULL) {
        printf("Memory allocation failed\n");
        free(threads);
        free(threadCreated);
        free(transactions);
        return 1;
    }

    pthread_mutex_init(&balanceMutex, NULL);

    for (i = 0; i < n; i++) {
        transactions[i].transactionId = i + 1;

        printf("\nTransaction %d\n", i + 1);
        printf("Enter type (1-Deposit, 2-Withdrawal, 3-Inquiry) : ");
        scanf("%d", &transactions[i].type);

        if (transactions[i].type == 1 || transactions[i].type == 2) {
            printf("Enter amount : ");
            scanf("%lf", &transactions[i].amount);
        } else {
            transactions[i].amount = 0.0;
        }

        if (pthread_create(&threads[i], NULL, processTransaction, &transactions[i]) != 0) {
            printf("Failed to create thread for transaction %d\n", i + 1);
        } else {
            threadCreated[i] = 1;
        }
    }

    for (i = 0; i < n; i++) {
        if (threadCreated[i] == 1) {
            pthread_join(threads[i], NULL);
        }
    }

    printf("\nFinal Account Balance : %.2f\n", accountBalance);
    printf("Total Successful Transactions : %d\n", successfulTransactions);

    pthread_mutex_destroy(&balanceMutex);
    free(threads);
    free(threadCreated);
    free(transactions);

    return 0;
}
