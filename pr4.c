//hands on with the DMA(Dynamic Memory Allocation) and struct pointer

#include<stdio.h>
#include<stdlib.h>

typedef struct Person
{
    char name[64];
    int age;
} Person;

int main()
{
    Person* p,*temp;
    int num,tempN=0;
    char choice;
    printf("Enter the number of person: ");
    scanf("%d", &num);
    p = malloc(num*sizeof(Person));
    if(p == NULL)
    {
        printf("Allocation failed!\n");
        return 1;
    }

    for(int i =0; i <num; i++)
    {

        printf("Enter the name: ");
        scanf(" %63[^\n]", p[i].name);
        printf("Enter the age: ");
        scanf("%d", &p[i].age);
    }

    printf("do you want to add more?(y/n) ");
    scanf(" %c", &choice);
    if(choice == 'y')
    {
        printf("Enter the number of additional person : ");
        scanf("%d", &tempN);
        temp = realloc(p,(num+tempN) * sizeof(Person));
        if(temp!=NULL)
        {
            p=temp;
            for(int i =num; i < num+tempN; i++)
            {
                printf("Enter the name: ");
                scanf(" %63[^\n]", p[i].name);
                printf("Enter the age: ");
                scanf("%d", &p[i].age);
            }
            num+=tempN;
        }
        else
        {
            printf("Reallocation failed!");
        }
    }

    for(int i =0; i < num; i++)
    {
        printf("%s - %d\n", p[i].name,p[i].age);
    }

    free(p);
    p=NULL;
    return 0;
}
