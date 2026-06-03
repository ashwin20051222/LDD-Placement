#include "stdio.h"

struct Student {
    int rollNumber;
    char name[50];
    float marks;
};

void update_marks(struct Student *student, float newMarks)
{
    student->marks = newMarks;
}

void print_student_by_value(struct Student student)
{
    printf("By value  : %d %s %.2f\n", student.rollNumber, student.name, student.marks);
}

void print_student_by_pointer(struct Student *student)
{
    printf("By pointer: %d %s %.2f\n", student->rollNumber, student->name, student->marks);
}

int main(void)
{
    struct Student student;
    float newMarks;

    printf("Enter roll number, name, and marks: ");
    scanf("%d %49s %f", &student.rollNumber, student.name, &student.marks);

    printf("Enter updated marks: ");
    scanf("%f", &newMarks);

    update_marks(&student, newMarks);
    print_student_by_value(student);
    print_student_by_pointer(&student);
    printf("Passing by pointer avoids copying the whole structure.\n");

    return 0;
}
