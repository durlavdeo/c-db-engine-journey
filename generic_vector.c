// A Generic Vector implementation to store any data type in a dynamic array!

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct
{
    void* data; //pointer of generic type(void) which will hold all the elements;
    size_t size; //will work as a next postion to store the element in the array.. also denotes the the number of elements currently in the array.
    size_t capacity; //denotes the capacity of an array.. will incrementally update it to reallocate the space when pushing back fills the array... see the line(40 & 41);
    size_t elementSize; //element size is added becuase we dont know what type of data the vector will be used for... 
                        //so we need to store the size of the data we are dealing with which makes it easier to compute the offsets and all.
} Vector;

//initialises the vector with size,capacity,element size, and dynamically allocates space for the array.
int init_vector(Vector *v,int capacity, int elemSize)
{
    if(capacity <=0 || elemSize <=0)
    {
        return -1;
    }

    v->data = malloc(capacity * elemSize);
    if(v->data == NULL)
    {
        return -2;
    }
    v->size = 0;
    v->capacity = capacity;
    v->elementSize = elemSize;
    return 0;
}

//push back to the array.
int push_back(Vector *v,void* val)
{
    if(v->size == v->capacity)
    {
        size_t newCap= v->capacity * 2;
        void* temp = realloc(v->data,newCap * v->elementSize); //reallocation because array is filled.
        if(temp!=NULL)
        {
            v->data = temp;
            v->capacity = newCap;
        }
        else
        {
            return -1;
        }
    }
    char *data = v->data;
    void* destination = data + (v->size * v->elementSize); //computing the offset
    memcpy(destination,val,v->elementSize); //copies the bytes from the val(src) to destination of size(v->elementSize)
    v->size++;
    return 0;

}

//get the element by index
int vector_get(Vector *v,int idx, void* result)
{
    char *data = v->data;
    void* src = data + (idx * v->elementSize);
    memcpy(result,src,v->elementSize);
    return 0;
}

//remove the last element in the array
int pop_back(Vector *v, void *result)
{
    char *data=v->data;
    void* src = data + (--v->size * v->elementSize);
    memcpy(result,src,v->elementSize); //copies the bytes from the src to destination(result) of size(v->elementSize)
    return 0;
}

//logially clear the array.. doesnt remove the elements in the array... it just resets the postion index(size) to overwrite the postions using push_back;
void clear(Vector *v)
{
    v->size = 0;
    return 0;
}

//deallocate the array and reset the vector.. also sets the array pointer to NULL to prevent dangling pointer;
void free_vector(Vector *v)
{
    if(v->data != NULL)
    {
        free(v->data);
        v->data = NULL;

    }
    v->capacity=v->size=v->elementSize = 0;
    return;
}

//removes an element at specified index;
void remove_at(Vector *v,int idx)
{
    char* data = v->data;
    void* src = data + ((idx+1) * v->elementSize);
    void* dest = data + (idx * v->elementSize);

    memmove(dest,src,(v->size-1-idx) * v->elementSize); //using memmove instead of memcpy beccuase memmove handles the overlapping which memcpy doesnt;
    v->size--;
}

//simply reallocating the larger space to the array .
void vector_reserve(Vector *v,int cap) {
    if(cap <= v->capacity){
        return;
    }

    void*temp = realloc(v->data,cap * v->elementSize );
    v->data = temp;
    v->capacity = cap;
    return;
}


//set the value at the index...
void set(Vector *v,int idx, void* value) {
    char*data=v->data;
    void* dest= data + (idx * v->elementSize);
    memcpy(dest,value,v->elementSize);
    return;
}

//a struct to test with..
typedef struct
{
    char name[64];
    int age;
} Person;

//you can check with other data types as well like ints doubles chars etc...
//simply declare vector like: Vector vector_name; init_vector(&vector_name, capacity, sizeof the datatype..{sizeof(int) | sizeof(double) | siezof(float) | ....})

int main()
{
    Vector ints;
    Person x= {"Durlav Deo",18},y= {"Durgesh Deo",19},result;
    init_vector(&ints, 2, sizeof(Person));
    push_back(&ints, &x);
    push_back(&ints, &y);

    for(int i = 0; i <2; i++)
    {
        vector_get(&ints,i,&result);
        printf("Name: %s \n", result.name);
        printf("age: %d \n", result.age);
    }

    return 0;
}
