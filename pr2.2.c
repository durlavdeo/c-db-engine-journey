//handling array pointer

#include<stdio.h>

void twoTime(int *arr)
{
    for(int i = 0; i<5; i++)
    {
        *(arr+i) = *(arr+i)*2;
    }
}

int main()
{

    int arr[5] = {1,2,3,4,5};
    printf("Before: " );
    for(int i=0; i<5; i++)
    {
        printf("%d ",arr[i]);
    }

    twoTime(arr);

    printf("\nAfter: " );
    for(int i=0; i<5; i++)
    {
        printf("%d ",arr[i]);
    }

    return 0;

}
