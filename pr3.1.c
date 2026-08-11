//struct pointer...
//hands on with struct and continuing pointer practise..


#include<stdio.h>
struct Student
{
    char name[100];
    int age;
    int marks;
};

void inputStudent(struct Student *std)
{

    printf("Enter name: ");
    scanf("%s", std->name);
    printf("Enter age: ");
    scanf("%d", &std->age);
    printf("Enter marks: ");
    scanf("%d", &std->marks);
}


void display(struct Student *std)
{
    printf("Name: %s\n", std->name);
    printf("Age: %d\n", std->age);
    printf("Marks: %d\n", std->marks);
}

void updateMarks(struct Student *std,int newMark)
{
    std->marks = newMark;
}

int main()
{

    struct Student std;
    inputStudent(&std);
    display(&std);
    updateMarks(&std,100);
    display(&std);

    return 0;

}
