#include "student.h"

#include <stdio.h>

void search_student(void)
{
    Student student;
    int roll_no;

    do {
        roll_no = read_int("Enter roll number to search: ");
        if (roll_no <= 0) {
            printf("Roll number must be greater than zero.\n");
        }
    } while (roll_no <= 0);

    if (!find_student_by_roll(roll_no, &student)) {
        printf("Student not found.\n");
        return;
    }

    printf("\nRoll No     : %d\n", student.roll_no);
    printf("Name        : %s\n", student.name);
    printf("Department  : %s\n", student.department);
    printf("Marks       : %.2f\n", student.marks);
}
