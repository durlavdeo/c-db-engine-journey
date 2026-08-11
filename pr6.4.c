//my ultimate project...connects all the dots from c fundamentals to structs to pointers to dma to file handling to binary file hadnling;
//will add comments for as much operations as i can;

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

//a struct to hold the student data at one place;
typedef struct
{
    int id;
    char name[100];
    int age;
} Student;

//counts the number of records in the file...got to learn about new funcitons : fseek and ftell;
int countStudents(FILE *fp,int *count)
{
    fseek(fp,0,SEEK_END); //simply moves the the cursor in the file at the end;
    long size = ftell(fp); // returns the total size from start to the current postion(which is the end of the file..hence returinign the total size of the file!);
    *count = size / sizeof(Student); //simple arithemtic to calculate number of records..

    return 0;
}

//get a single student...got to understand the random access of the file using fseek;
int getStudent(FILE *fp, int idx, Student *s)
{
    int count = 0;
    countStudents(fp,&count);
    if(idx < 0 || idx > count -1)
    {

        printf("Invalid Index!");
        return -1;
    }

    fseek(fp, idx * sizeof(Student),SEEK_SET); //moves the cursor to the idx postion from the start of the file...multiplication is done to jump that offset because we are storing a fixeed size records..
    fread(s,sizeof(Student),1,fp); 

    return 0;

}

//updates the single record...simply overwrites the record after reaching the postion by using fseek with offset;
int updateStudent(FILE *fp,int idx, Student *s)
{
    long offset = idx * sizeof(Student);
    fseek(fp,offset,SEEK_SET);

    fwrite(s,sizeof(Student),1,fp);
    return 0;
}


//adds a record;
int addStudent(FILE *fp, Student *s)
{
    fseek(fp,0,SEEK_END);
    fwrite(s,sizeof(Student),1,fp);
    return 0;
}

//this one was so cool to implement and also took the longest(ofcourse!)..
//so what it simply does is shifts the records one by one to the left and then the file is truncated to the one less record...
// becuase after shifting every records to the left, the last and second last record will be identical and so we truncate the last record...and it physcailly shrinks the size of the file..
int deleteStudent(FILE *fp,int idx)
{
    int count = 0;
    countStudents(fp,&count);
    if(count==0)
    {
        printf("Empty File!");
        return -1;
    }
    if(idx < 0 || idx > count -1)
    {

        printf("Invalid Index!");
        return -1;
    }

    Student s;

    //simply going to the second last index.. becuase if we are to delete the last record we will simply truncate the file as done after line(93)
    while(idx < count - 1)
    {
        fseek(fp,++idx * sizeof(Student),SEEK_SET); //move one record right to the current index...
        fread(&s,sizeof(Student),1,fp);//copy the current record to the s;
        fseek(fp,--idx * sizeof(Student),SEEK_SET);//move back one record;
        fwrite(&s,sizeof(Student),1,fp);//overwrite with the s;
        idx++;//update the idx to next;
    }
    long truncateSize = (count - 1) * sizeof(Student); 
    if (fflush(fp) != 0) //to flush out C FILE* buffer;
    {
        perror("fflush");
        return -1;
    }

    if (ftruncate(fileno(fp), truncateSize) != 0) //finally truncate the file.. the fileno() returns the file descriptor of fp;
    {
        perror("ftruncate");
        return -1;
    }
    return 0;

}



//list students;
int printStudents(FILE *fp)
{
    Student s;
    fseek(fp,0,SEEK_SET);
    while((fread(&s,sizeof(Student),1,fp)) == 1)
    {
        printf("%d,%s,%d\n",s.id,s.name,s.age);
    }
    return 0;
}

int main()
{
    Student students[5] =
    {
        {1,"Durlav Deo",18},
        {2,"Durgesh Deo",18},
        {3,"MD Amir",18},
        {4,"Abinash Mehta",18},
        {5,"Sharwendra Sharma",16}
    };

    FILE *fp = fopen("std.dat","w+b");
    if(fp == NULL)
    {
        printf("Error opening file!");
        exit(1);
    }


    fwrite(students,sizeof(Student),sizeof(students)/sizeof(Student),fp);
    printf("Before:\n");
    printStudents(fp);

    fclose(fp);

    return 0;
}
