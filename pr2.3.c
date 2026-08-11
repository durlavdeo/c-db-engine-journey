//char pointer

#include<stdio.h>

int strlen(char *str)
{
    int len=0;
    for(int i=0; str[i] !='\0'; i++)
    {
        len++;
    }

    return len;
}


int main()
{
    char str[100];
    printf("Enter the string: ");
    scanf("%s", str);
    int l = strlen(str);

    printf("The length of the string is : %d", l);
    return 0;
}
