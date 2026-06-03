#include "stdio.h"

struct Employee {
    int id;
    char name[50];
    double salary;
};

int main(void)
{
    struct Employee employees[10];
    double totalSalary = 0.0;
    int highestIndex = 0;
    int i;

    for (i = 0; i < 10; i++) {
        printf("Enter id, name, and salary for employee %d: ", i + 1);
        scanf("%d %49s %lf", &employees[i].id, employees[i].name, &employees[i].salary);

        totalSalary = totalSalary + employees[i].salary;
        if (employees[i].salary > employees[highestIndex].salary) {
            highestIndex = i;
        }
    }

    printf("Total salary payout = %.2f\n", totalSalary);
    printf("Employee with highest salary:\n");
    printf("ID: %d\n", employees[highestIndex].id);
    printf("Name: %s\n", employees[highestIndex].name);
    printf("Salary: %.2f\n", employees[highestIndex].salary);

    return 0;
}
