//combining all the practise of pointers, dma and structs to create an int vector;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Vect
{
    int* data;
    int size;
    int capacity;
} Vector;

int init_vector(Vector *v, int cap)
{
    v->data = malloc(cap * sizeof(int));
    if(v->data == NULL)
    {
        return -1;
    }
    v->size = 0;
    v->capacity = cap;
    return 0;
}

int push_back(Vector *v, int value)
{

    if(v->size == v->capacity)
    {

        int newCap = v->capacity * 2;
        int *temp = realloc(v->data, newCap * sizeof(int));
        if(temp != NULL)
        {
            v->data = temp;
            v->capacity = newCap;
            printf("Reallocation success!\n");
        }
        else
        {
            printf("Reallocation failed!\n");
            return -1;
        }
    }

    v->data[v->size++] = value;
    printf("Value pushed: %d\n",value);
    return 0;
}


int pop_back(Vector *v, int *result)
{
    if(v->size <=0)
    {
        printf("pop failed!\n");
        return -1;
    }

    *result = v->data[--v->size];
    printf("Result : %d\n",*result);
    return 0;
}

int get(Vector *v,int idx, int* result)
{
    if(idx>=0 && idx<v->size)
    {

        *result = v->data[idx];
        printf("Get: %d\n",*result);
        return 0;
    }
    printf("Invalid index: %d\n",idx);
    return -1;
}
int set(Vector *v,int idx, int value)
{
    if(idx>=0 && idx<v->size)
    {

        v->data[idx] = value;
        printf("Set %d at %d idx\n",value,idx);
        return 0;
    }
    printf("Invalid index: %d\n",idx);
    return -1;
}


int vector_reserve(Vector *v, int cap)
{
    if(cap > v->capacity)
    {
        int *temp = realloc(v->data,cap*sizeof(int));
        if(temp != NULL)
        {
            v->data = temp;
            v->capacity = cap;
        }
        else
        {

            return -1;
        }

    }
    return 0;
}
int is_Empty(Vector *v)
{
    return v->size == 0;
}

int clear_vector(Vector *v)
{
    v->size = 0;
    return 0;
}

int remove_at(Vector *v, int idx)
{
    if(idx >=0 && idx < v->size)
    {
        memmove(&v->data[idx],&v->data[idx+1],(v->size-1-idx) * sizeof(int));
        v->size--;
        return 0;
    }
    return -1;
}

void free_vector(Vector *v)
{
    if(v->data != NULL)
    {
        free(v->data);

        v->data = NULL;
    }
    v->size = v->capacity = 0;
    printf("Vector deallocated!\n");
}

void print_vector(Vector *v)
{
    printf("Data: ");
    for(int i=0; i<v->size; i++)
    {
        printf("%d ",v->data[i]);
    }
    printf("Size: %d\n",v->size);
    printf("Capacity: %d\n", v->capacity);
}


int main()
{
    Vector v;
    int cap,result;
    printf("================CUSTOM VECTOR================\n");
init:
    printf("Enter the capacity of the vector(>0): ");
    scanf("%d", &cap);
    if(cap<=0)
    {
        printf("Invalid capacity!\n");
        goto init;
    }
    int status = init_vector(&v, cap);
    if(status == 0)
    {
        printf("Vector initialization successful!\n");
    }
    else
    {
        printf("Vector initialization failed!\n");
        return 1;
    }

    free_vector(&v);
    return 0;
}
