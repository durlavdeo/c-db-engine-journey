//single page
//i am writing about what page is as per i understood...
//so a page is a collection of records of fixed size...a page itself has a fixed page and a file have multiple pages...The concept of page is introduced to organize and manage the records more efficiently and effectively..
//page allows to retrieve the chunk of data from the file to minimize the storage I/O access because the storage I/O access is expensive...the chunk of data(page) retrieved is stored in the memory(cache pool or pool buffer)...
//the meta data in the pages contain information such as the number of records, number of free slots , and total free space....
//we will finally do bit mapping which will indicate the whether a slot is free or used....
//this is what i understood so far about pages...not so much but enough to start working with it...

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define PAGE_SIZE 256

typedef struct
{
    char data[PAGE_SIZE];
} Page;


int main()
{
    Page p;
    int ints[] = {11,22,33,44,55};
    memcpy(p.data,ints,sizeof(ints));
    FILE *fp = fopen("page.dat","w+b"); //using w+b mode to reuse the same file for the tests...
    if(fp == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }

    fwrite(&p,sizeof(Page),1,fp);
    fseek(fp,0,SEEK_SET);

    Page loadedPage;

    fread(&loadedPage,sizeof(Page),1,fp);
    int loadedNums[5];
    memcpy(loadedNums,loadedPage.data,sizeof(loadedNums));

    for(int i=0;i<5;i++)
    {
        printf("%d ",loadedNums[i]);
    }

    fclose(fp);


    return 0;
}
