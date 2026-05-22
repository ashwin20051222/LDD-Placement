#include "student.h"

#include <stdio.h>

static int is_valid_student_record(const Student *student)
{
    return student != NULL &&
           student->roll_no > 0 &&
           student->name[0] != '\0' &&
           student->department[0] != '\0' &&
           student->marks >= 0.0f &&
           student->marks <= 100.0f;
}

int append_student(const Student *student)
{
    FILE *file = fopen(STUDENT_DB_FILE, "ab");

    if (!is_valid_student_record(student)) {
        printf("Invalid student record. Record was not saved.\n");
        return 0;
    }

    if (file == NULL) {
        perror("Unable to open student database");
        return 0;
    }

    if (fwrite(student, sizeof(*student), 1, file) != 1) {
        perror("Unable to write student record");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int find_student_by_roll(int roll_no, Student *student)
{
    FILE *file = fopen(STUDENT_DB_FILE, "rb");

    if (file == NULL) {
        return 0;
    }

    while (fread(student, sizeof(*student), 1, file) == 1) {
        if (is_valid_student_record(student) && student->roll_no == roll_no) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int read_all_students(Student students[], int max_students)
{
    FILE *file = fopen(STUDENT_DB_FILE, "rb");
    int count = 0;

    if (file == NULL) {
        return 0;
    }

    while (count < max_students &&
           fread(&students[count], sizeof(students[count]), 1, file) == 1) {
        if (is_valid_student_record(&students[count])) {
            count++;
        }
    }

    fclose(file);
    return count;
}
