/* student_fixed.c
 Fixed-size student records and direct access using fseek/ftell
 Compile: gcc -o student_fixed student_fixed.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int roll;
    char name[50];
    float marks;
} Stu;

void add(const char *file)
{
    Stu s;

    printf("Enter roll: ");
    if (scanf("%d", &s.roll) != 1)
        return;

    printf("Enter name: ");
    scanf(" %49[^\\n]", s.name);

    printf("Enter marks: ");
    if (scanf("%f", &s.marks) != 1)
        return;

    FILE *f = fopen(file, "ab");
    if (!f) {
        perror("fopen");
        return;
    }

    fwrite(&s, sizeof(s), 1, f);
    fclose(f);

    printf("Saved.\n");
}

void show_nth(const char *file)
{
    int n;
    printf("Enter record index (0-based): ");
    if (scanf("%d", &n) != 1)
        return;

    FILE *f = fopen(file, "rb");
    if (!f) {
        printf("No file.\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    int total = (int)(size / sizeof(Stu));

    printf("Total records = %d\n", total);

    if (n < 0 || n >= total) {
        printf("Out of range.\n");
        fclose(f);
        return;
    }

    fseek(f, n * sizeof(Stu), SEEK_SET);

    Stu s;
    fread(&s, sizeof(s), 1, f);

    printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);

    fclose(f);
}

void list_all(const char *file)
{
    FILE *f = fopen(file, "rb");
    if (!f) {
        printf("No file.\n");
        return;
    }

    Stu s;
    printf("Roll\tName\tMarks\n");

    while (fread(&s, sizeof(s), 1, f) == 1) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(f);
}

int main(void)
{
    const char *file = "students_fixed.dat";
    int c = 0;

    while (1) {
        printf("\n1.Add 2.Show Nth 3.List 4.Exit\nChoice: ");
        if (scanf("%d", &c) != 1)
            break;

        if (c == 1)
            add(file);
        else if (c == 2)
            show_nth(file);
        else if (c == 3)
            list_all(file);
        else
            break;
    }

    return 0;
}
