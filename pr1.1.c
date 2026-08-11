//getting hands on with the c fundamentals

#include<stdio.h>
int main()
{
    int choice;
    float result,a,b;
    char cont = 'y';
    do
    {
        result =0.0;
        printf("Select the operation: \n");
        printf("1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n");
        scanf("%d", &choice);

        if(choice >=1 && choice <=4)
        {
            printf("Enter first operand: ");
            scanf("%f", &a);
            printf("Enter second operand: ");
            scanf("%f", &b);

            switch(choice)
            {
            case 1:
                result = a+b;
                break;
            case 2:
                result = a-b;
                break;
            case 3:
                result = a*b;
                break;
            case 4:
                if(b==0)
                {
                    printf("Error: division by zero\n");
                }
                else
                {
                    result = a/b;
                }
                break;

            }
            printf("The result is : %0.2f\n",result);

        }
        else
        {
            printf("Invalid choice!\n");
        }
        printf("DO you want to continue?(y/n)");
        scanf(" %c", &cont);

    }
    while(cont == 'y');

    return 0;
}
