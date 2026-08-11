//getting hands on with the c fundamentals

#include<stdio.h>

int main()
{
    float temp,result;
    int choice;
    char cont = 'y';
    do
    {
        printf("Convert:\n1.F->C\n2.C->F\n");
        scanf("%d",&choice);
        if(choice < 1 || choice >2)
        {
            printf("Invalid choice!\n");
        }
        else
        {
            printf("Enter temperature: ");
            scanf("%f", &temp);

            switch(choice)
            {
            case 1:
                result = ((temp-32)*(5.0/9.0));
                break;
            case 2:
                result = ((9.0/5.0)*temp) + 32;
                break;

            }
            printf("Result: %0.2f\n",result);
        }
        printf("Do you want to continue?(y/n)");
        scanf(" %c", &cont);
    }
    while(cont == 'y');

    return 0;
}
