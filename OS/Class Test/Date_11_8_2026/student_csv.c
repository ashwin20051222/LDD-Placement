/* student_csv.c
 Simple Student Management using struct array and CSV file
 Compile: gcc -o student_csv student_csv.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int roll;
    char name[50];
    float marks;
} Student;

void add_student(const char *file) {
    Student s;
    printf("Enter roll: "); if(scanf("%d", &s.roll)!=1) return;
    printf("Enter name: "); scanf(" %49[^\\n]", s.name);
    printf("Enter marks: "); if(scanf("%f", &s.marks)!=1) return;
    FILE *f = fopen(file, "a");
    if(!f){ perror("fopen"); return; }
    fprintf(f, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
    fclose(f);
    printf("Added.\n");
}

void list_students(const char *file) {
    FILE *f = fopen(file, "r");
    if(!f){ printf("No students yet.\n"); return; }
    Student s; char line[200];
    printf("Roll\tName\tMarks\n");
    while(fgets(line, sizeof(line), f)){
        if(sscanf(line, "%d,%49[^,],%f", &s.roll, s.name, &s.marks)==3)
            printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }
    fclose(f);
}

void update_student(const char *file) {
    int target; printf("Enter roll to update: "); if(scanf("%d", &target)!=1) return;
    FILE *f = fopen(file, "r");
    if(!f){ printf("No data.\n"); return; }
    Student a[500]; int n=0; char line[200];
    while(fgets(line, sizeof(line), f)){
        if(sscanf(line, "%d,%49[^,],%f", &a[n].roll, a[n].name, &a[n].marks)==3) n++;
    }
    fclose(f);
    int found=0;
    for(int i=0;i<n;i++) if(a[i].roll==target){
        printf("Enter new name: "); scanf(" %49[^\\n]", a[i].name);
        printf("Enter new marks: "); if(scanf("%f", &a[i].marks)!=1) return;
        found=1; break;
    }
    if(!found){ printf("Not found.\n"); return; }
    f = fopen(file, "w"); if(!f){ perror("fopen"); return; }
    for(int i=0;i<n;i++) fprintf(f, "%d,%s,%.2f\n", a[i].roll, a[i].name, a[i].marks);
    fclose(f); printf("Updated.\n");
}

void delete_student(const char *file) {
    int target; printf("Enter roll to delete: "); if(scanf("%d", &target)!=1) return;
    FILE *f = fopen(file, "r");
    if(!f){ printf("No data.\n"); return; }
    Student a[500]; int n=0; char line[200];
    while(fgets(line, sizeof(line), f)){
        if(sscanf(line, "%d,%49[^,],%f", &a[n].roll, a[n].name, &a[n].marks)==3) n++;
    }
    fclose(f);
    int found=0;
    FILE *out = fopen("tmp.csv","w"); if(!out){ perror("fopen"); return; }
    for(int i=0;i<n;i++){
        if(a[i].roll==target) { found=1; continue; }
        fprintf(out, "%d,%s,%.2f\n", a[i].roll, a[i].name, a[i].marks);
    }
    fclose(out);
    if(found){ remove(file); rename("tmp.csv", file); printf("Deleted.\n"); }
    else { remove("tmp.csv"); printf("Not found.\n"); }
}

int main(){
    const char *file = "students.csv";
    int choice;
    while(1){
        printf("\n1.Add 2.List 3.Update 4.Delete 5.Exit\nChoose: ");
        if(scanf("%d", &choice)!=1) break;
        if(choice==1) add_student(file);
        else if(choice==2) list_students(file);
        else if(choice==3) update_student(file);
        else if(choice==4) delete_student(file);
        else break;
    }
    return 0;
}
