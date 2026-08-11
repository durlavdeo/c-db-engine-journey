//getting hands on with the c fundamentals

#include<stdio.h>

int main()
{
    char str[100],result[100];int len=0;

    printf("Enter the string: ");
    scanf("%s", str);

    for(int i =0; str[i]!='\0';i++){
        len++;
    }

    for(int i = len-1,j=0; i >=0;i--,j++){
        result[j] =
        str[i];
    }

    result[len] = '\0';

    printf("The reverse is: %s", result);

    return 0;
}
