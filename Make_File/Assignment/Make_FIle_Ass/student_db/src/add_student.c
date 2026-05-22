#include "student.h"

#include <stdio.h>

void add_student(void)
{
    Student student;

#ifdef DEBUG
    printf("[debug] using database file: %s\n", STUDENT_DB_FILE);
#endif

    do {
        student.roll_no = read_int("Roll number: ");
        if (student.roll_no <= 0) {
            printf("Roll number must be greater than zero.\n");
        }
    } while (student.roll_no <= 0);

    if (find_student_by_roll(student.roll_no, &student)) {
        printf("A student with this roll number already exists.\n");
        return;
    }

    do {
        read_line("Name: ", student.name, MAX_NAME_LEN);
        if (student.name[0] == '\0') {
            printf("Name cannot be empty.\n");
        }
    } while (student.name[0] == '\0');

    do {
        read_line("Department: ", student.department, MAX_DEPARTMENT_LEN);
        if (student.department[0] == '\0') {
            printf("Department cannot be empty.\n");
        }
    } while (student.department[0] == '\0');

    do {
        student.marks = read_float("Marks: ");
        if (student.marks < 0.0f || student.marks > 100.0f) {
            printf("Marks must be between 0 and 100.\n");
        }
    } while (student.marks < 0.0f || student.marks > 100.0f);

    if (append_student(&student)) {
        printf("Student added successfully.\n");
    }
}
