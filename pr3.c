#include<stdio.h>

struct Person{
    char name[100];
    int age;
};

int main(){
    struct Person p[5];

    printf("Enter the details of the person: \n");
    for(int i =0; i<5;i++){
        printf("Enter the name: ");
        scanf(" %s", p[i].name);
        printf("Enter the age: ");
        scanf("%d", &p[i].age);
    }
    printf("Details: \n");
    printf("Name\tAge\n");
    for(int i =0; i<5;i++){
        printf("%s\t%d\n",p[i].name,p[i].age);
    }

    return 0;
}
