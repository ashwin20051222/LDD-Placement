#include "stdio.h"
#include "stdlib.h"

typedef struct {
    int *data;
    int size;
    int capacity;
} Vector;

int vector_init(Vector *vector)
{
    vector->size = 0;
    vector->capacity = 4;
    vector->data = malloc((unsigned long)vector->capacity * sizeof(int));

    return vector->data != 0;
}

int vector_resize(Vector *vector, int newCapacity)
{
    int *newData;

    if (newCapacity < vector->size) {
        return 0;
    }

    newData = realloc(vector->data, (unsigned long)newCapacity * sizeof(int));
    if (newData == 0 && newCapacity > 0) {
        return 0;
    }

    vector->data = newData;
    vector->capacity = newCapacity;
    return 1;
}

int vector_push(Vector *vector, int value)
{
    if (vector->size == vector->capacity) {
        if (!vector_resize(vector, vector->capacity * 2)) {
            return 0;
        }
    }

    vector->data[vector->size] = value;
    vector->size++;
    return 1;
}

int vector_pop(Vector *vector, int *value)
{
    if (vector->size == 0) {
        return 0;
    }

    vector->size--;
    *value = vector->data[vector->size];
    return 1;
}

void vector_print(Vector *vector)
{
    int i;

    printf("Vector: ");
    for (i = 0; i < vector->size; i++) {
        printf("%d ", vector->data[i]);
    }
    printf("\n");
}

void vector_free(Vector *vector)
{
    free(vector->data);
    vector->data = 0;
    vector->size = 0;
    vector->capacity = 0;
}

int main(void)
{
    Vector vector;
    int value;

    if (!vector_init(&vector)) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    vector_push(&vector, 10);
    vector_push(&vector, 20);
    vector_push(&vector, 30);
    vector_print(&vector);

    if (vector_pop(&vector, &value)) {
        printf("Popped value = %d\n", value);
    }

    vector_resize(&vector, 8);
    printf("Capacity after resize = %d\n", vector.capacity);
    vector_print(&vector);

    vector_free(&vector);
    return 0;
}
