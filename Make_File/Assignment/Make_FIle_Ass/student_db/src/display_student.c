#include "student.h"

#include <stdio.h>

#define MAX_STUDENTS 1000

void display_students(void)
{
    Student students[MAX_STUDENTS];
    int count = read_all_students(students, MAX_STUDENTS);
    int i;

    if (count == 0) {
        printf("No student records found.\n");
        return;
    }

    printf("%-10s %-24s %-24s %s\n", "Roll No", "Name", "Department", "Marks");
    printf("---------------------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-10d %-24s %-24s %.2f\n",
               students[i].roll_no,
               students[i].name,
               students[i].department,
               students[i].marks);
    }
}
