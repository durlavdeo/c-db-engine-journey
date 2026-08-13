//multiple pages...with different data type per page...

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PAGE_SIZE 256


typedef struct
{
    char data[256];
} Page;

typedef struct
{
    int id;
    char name[100];
    int age;
} Student;

int main()
{
    FILE *fp=fopen("page2.dat","w+b"); //using w+b mode to reuse the same file for the tests...

    if(fp==NULL)
    {
        printf("Error opening file!");
        exit(1);
    }

    Page p = {0};

    int data[] = {11,22,33,44,55};
    Student s = {1,"Durlav Deo",19};
    //write to the file...
    memcpy(p.data,data,sizeof(data));
    fwrite(&p,sizeof(Page),1,fp);
    memset(p.data,0,sizeof(Page)); //resets the p.data..removes the previous ints...
    memcpy(p.data,&s,sizeof(Student));
    fwrite(&p,sizeof(Page),1,fp);

    //read pages from the file...
    Page p0,p1;
    fseek(fp,0,SEEK_SET);
    fread(&p0,sizeof(Page),1,fp);
    fseek(fp,1*PAGE_SIZE,SEEK_SET);
    fread(&p1,sizeof(Page),1,fp);

    printf("Page 0:\n");
    int loadedData[5];
    memcpy(loadedData,p0.data,sizeof(loadedData));
    for(int i=0; i<5; i++)
    {
        printf("%d ",loadedData[i]);
    }

    printf("\nPage 1:\n");
    Student loadedStudent;
    memcpy(&loadedStudent,p1.data,sizeof(Student));
    printf("ID: %d\nName: %s\nAge: %d\n",loadedStudent.id,loadedStudent.name,loadedStudent.age);

    fclose(fp);
    return 0;
}
