//practising file handling to store structured data... hands on with the functions like fprintf and sscanf...
//sscanf allows to parse the string in the specified format...line(56)
//menu like interface to perform operations...

#include<stdio.h>
#include<stdlib.h>

typedef struct
{
    int id;
    char name[100];
    float marks;

} Student;

void addStudent()
{
    Student s;
    printf("\n===========================ADD STUDENT===========================\n");
    printf("\nEnter id: ");
    scanf("%d", &s.id);
    printf("Enter name: ");
    scanf(" %99[^\n]", s.name);
    printf("Enter marks: ");
    scanf("%f", &s.marks);


    FILE *fp = fopen("student.txt","a");
    if(fp == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }
    fprintf(fp,"%d,%s,%.2f\n",s.id,s.name,s.marks);
    printf("Student added successfully!");
    printf("\n=================================================================\n");
    fclose(fp);
}

void listStudents()
{
    printf("\n==========================STUDENTS LIST==========================\n");
    FILE *fp = fopen("student.txt","r");
    if(fp == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }

    char buffer[200];
    Student s;
    printf("\n");
    printf("ID\tNAME\tMARKS\n");
    while((fgets(buffer,sizeof(buffer),fp)) != NULL )
    {
        int result = sscanf(buffer,"%d,%99[^,],%f",&s.id,s.name,&s.marks);
        if(result  !=3 ){
            printf("Malformed Record: %s", buffer);
            continue;
        }
        printf("%d\t%s\t%.2f\n",s.id,s.name,s.marks);

    }

    printf("\n=================================================================\n");
    fclose(fp);
}

void searchStudent()
{
    int id;
    char buffer[200];
    Student s;
    int found = 0;
    printf("Enter the id: ");
    scanf("%d", &id);
    FILE *fp = fopen("student.txt","r");
    if(fp == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }
    while((fgets(buffer,sizeof(buffer),fp)) != NULL )
    {
        int result = sscanf(buffer,"%d,%99[^,],%f",&s.id, s.name, &s.marks);
        if(result != 3 )
        {
            printf("Skipping malformed record: %s", buffer);
            continue;
        }
        if(s.id == id)
        {
            found = 1;
            break;
        }
    }

    if(found == 1)
    {
        printf("The student is found!\n");
        printf("ID : %d\n", s.id);
        printf("Name : %s\n", s.name);
        printf("Marks : %.2f\n", s.marks);
    }
    else
    {
        printf("Student not found!");

    }

    fclose(fp);


}



int main()
{
    int choice;
    printf("===========================STUDENT DB===========================\n");
    while(1)
    {
        printf("\nChoose:\n");
        printf("1.Add Student\n2.List Students\n3.Search Student\n4.Exit\n");
        printf("\n/>");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            listStudents();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            exit(0);
        default:
            printf("Invalid choice!");
            break;
        }

    }

    return 0;

}
