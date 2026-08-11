//BINARY FILE HANDLINGGGGG!!!!!!!!!!

#include<stdio.h>
#include<stdlib.h>

typedef struct
{
    int id;
    char name[100];
    float marks;
} Student;


int main()
{
    Student students[] =
    {
        {1, "Durlav Deo", 98.65},
        {2, "Ram Sharma", 82.50},
        {3, "Sita Thapa", 91.25}
    };
    Student s2;
    FILE *fp = fopen("student.dat", "wb");
    fwrite(students,sizeof(Student),sizeof(students)/sizeof(Student),fp);
    fclose(fp);
    fp=fopen("student.dat","rb");
    while((fread(&s2,sizeof(Student),1,fp)) == 1)
    {
        printf("%d,%s,%.2f\n",s2.id,s2.name,s2.marks);
    }
    return 0;
}
