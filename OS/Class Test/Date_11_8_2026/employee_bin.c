/* employee_bin.c
 Employee Management using binary file with in-place update
 Compile: gcc -o employee_bin employee_bin.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float salary;
} Emp;

void add_emp(const char *f)
{
    Emp e;

    printf("Enter id: ");
    if (scanf("%d", &e.id) != 1)
        return;

    printf("Enter name: ");
    scanf(" %49[^\\n]", e.name);

    printf("Enter salary: ");
    if (scanf("%f", &e.salary) != 1)
        return;

    FILE *fp = fopen(f, "ab");
    if (!fp) {
        perror("fopen");
        return;
    }

    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);

    printf("Added.\n");
}

void list_emp(const char *f)
{
    FILE *fp = fopen(f, "rb");
    if (!fp) {
        printf("No file.\n");
        return;
    }

    Emp e;
    printf("ID\tName\tSalary\n");

    while (fread(&e, sizeof(e), 1, fp) == 1) {
        printf("%d\t%s\t%.2f\n", e.id, e.name, e.salary);
    }

    fclose(fp);
}

void update_emp(const char *f)
{
    int id;
    printf("Enter id to update: ");
    if (scanf("%d", &id) != 1)
        return;

    FILE *fp = fopen(f, "r+b");
    if (!fp) {
        printf("No file.\n");
        return;
    }

    Emp e;
    long pos;
    int found = 0;

    while (fread(&e, sizeof(e), 1, fp) == 1) {
        if (e.id == id) {
            printf("Enter new name: ");
            scanf(" %49[^\\n]", e.name);

            printf("Enter new salary: ");
            if (scanf("%f", &e.salary) != 1) {
                fclose(fp);
                return;
            }

            pos = ftell(fp) - sizeof(e);
            fseek(fp, pos, SEEK_SET);
            fwrite(&e, sizeof(e), 1, fp);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
        printf("Updated in place.\n");
    else
        printf("Not found.\n");
}

void delete_emp(const char *f)
{
    int id;
    printf("Enter id to delete: ");
    if (scanf("%d", &id) != 1)
        return;

    FILE *fp = fopen(f, "rb");
    if (!fp) {
        printf("No file.\n");
        return;
    }

    Emp e;
    FILE *out = fopen("tmp.dat", "wb");
    if (!out) {
        fclose(fp);
        perror("fopen");
        return;
    }

    int found = 0;
    while (fread(&e, sizeof(e), 1, fp) == 1) {
        if (e.id == id) {
            found = 1;
            continue;
        }
        fwrite(&e, sizeof(e), 1, out);
    }

    fclose(fp);
    fclose(out);

    if (found) {
        remove(f);
        rename("tmp.dat", f);
        printf("Deleted.\n");
    } else {
        remove("tmp.dat");
        printf("Not found.\n");
    }
}

int main(void)
{
    const char *file = "employees.dat";
    int ch = 0;

    while (1) {
        printf("\n1.Add 2.List 3.Update 4.Delete 5.Exit\nChoice: ");
        if (scanf("%d", &ch) != 1)
            break;

        if (ch == 1)
            add_emp(file);
        else if (ch == 2)
            list_emp(file);
        else if (ch == 3)
            update_emp(file);
        else if (ch == 4)
            delete_emp(file);
        else
            break;
    }

    return 0;
}
