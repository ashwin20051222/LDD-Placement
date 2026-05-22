#include "stdio.h"

struct Student {
    int rollNumber;
    char name[50];
    float marks;
};

int main(void)
{
    struct Student students[5];
    int topperIndex = 0;
    int i;

    for (i = 0; i < 5; i++) {
        printf("Enter roll number, name, and marks for student %d: ", i + 1);
        scanf("%d %49s %f", &students[i].rollNumber, students[i].name, &students[i].marks);

        if (students[i].marks > students[topperIndex].marks) {
            topperIndex = i;
        }
    }

    printf("Topper details:\n");
    printf("Roll Number: %d\n", students[topperIndex].rollNumber);
    printf("Name: %s\n", students[topperIndex].name);
    printf("Marks: %.2f\n", students[topperIndex].marks);

    return 0;
}
