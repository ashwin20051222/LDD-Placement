#ifndef STUDENT_H
#define STUDENT_H

#define STUDENT_DB_FILE "students.dat"
#define MAX_NAME_LEN 64
#define MAX_DEPARTMENT_LEN 64

typedef struct {
    int roll_no;
    char name[MAX_NAME_LEN];
    char department[MAX_DEPARTMENT_LEN];
    float marks;
} Student;

void add_student(void);
void search_student(void);
void display_students(void);

int append_student(const Student *student);
int find_student_by_roll(int roll_no, Student *student);
int read_all_students(Student students[], int max_students);

void clear_input_buffer(void);
void read_line(const char *prompt, char *buffer, int size);
int read_int(const char *prompt);
float read_float(const char *prompt);

#endif
