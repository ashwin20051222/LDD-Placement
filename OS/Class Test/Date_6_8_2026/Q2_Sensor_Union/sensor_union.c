#include <stdio.h>
#include <string.h>

/*
 * Sensor storage using union for reading (either int or float).
 * Demonstrates passing union by value and by pointer.
 */

typedef union {
    int i;
    float f;
} reading_t;

typedef struct {
    int id;
    char name[50];
    int type; /* 1 = int, 2 = float */
    reading_t value;
} sensor_t;

/* Read sensor details from user (by pointer) */
void readSensor(sensor_t *s)
{
    printf("Enter Sensor ID : ");
    scanf("%d", &s->id);
    getchar();

    printf("Enter Sensor Name : ");
    fgets(s->name, sizeof s->name, stdin);
    s->name[strcspn(s->name, "\n")] = '\0';

    printf("\nChoose Reading Type\n1.Integer\n2.Float\nChoice : ");
    scanf("%d", &s->type);

    if (s->type == 1) {
        printf("%s = ", s->name);
        scanf("%d", &s->value.i);
    } else {
        printf("%s = ", s->name);
        scanf("%f", &s->value.f);
    }
}

/* Display sensor (pass-by-value) */
void displaySensor(sensor_t s)
{
    printf("\nSensor Details\n\n");
    printf("ID :%d\n", s.id);
    printf("Name :%s\n", s.name);
    if (s.type == 1) {
        printf("Reading :%d\n", s.value.i);
    } else {
        printf("Reading :%.2f\n", s.value.f);
    }
}

/* Update sensor reading (by pointer) */
void updateSensor(sensor_t *s)
{
    printf("\nUpdating Reading...\n");
    if (s->type == 1) {
        printf("Enter new integer value : ");
        scanf("%d", &s->value.i);
        printf("Updated Value :%d\n", s->value.i);
    } else {
        printf("Enter new float value : ");
        scanf("%f", &s->value.f);
        printf("Updated Value :%.2f\n", s->value.f);
    }
}

/* Compare two sensors: by id first, then by reading value */
int compareSensor(sensor_t a, sensor_t b)
{
    if (a.id != b.id)
        return (a.id < b.id) ? -1 : 1;

    if (a.type != b.type)
        return a.type - b.type;

    if (a.type == 1) {
        if (a.value.i == b.value.i) return 0;
        return (a.value.i < b.value.i) ? -1 : 1;
    } else {
        if (a.value.f == b.value.f) return 0;
        return (a.value.f < b.value.f) ? -1 : 1;
    }
}

/* Reset sensor */
void resetSensor(sensor_t *s)
{
    s->id = 0;
    s->name[0] = '\0';
    s->type = 1;
    s->value.i = 0;
}

int main(void)
{
    sensor_t s1, s2;

    /* Read first sensor */
    readSensor(&s1);
    displaySensor(s1); /* pass-by-value example */

    /* Update the sensor by pointer */
    updateSensor(&s1);

    /* Display updated */
    displaySensor(s1);

    /* Demonstrate compare and reset */
    printf("\nCreate a second sensor to compare\n");
    readSensor(&s2);

    int cmp = compareSensor(s1, s2);
    if (cmp == 0)
        printf("Sensors are equal\n");
    else if (cmp < 0)
        printf("Sensor 1 < Sensor 2\n");
    else
        printf("Sensor 1 > Sensor 2\n");

    resetSensor(&s2);
    printf("\nSecond sensor after reset:\n");
    displaySensor(s2);

    return 0;
}
