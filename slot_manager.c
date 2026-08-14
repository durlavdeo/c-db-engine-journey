//so this is the slot manager program...it simply is the implementation of something known as bit map...the bit map is a clever idea to store the status of slots in the pages in the compact form..in terms if bits for each slots...
// where each bit position means the slot number...the set/clear of bits represents either the slot is in use or empty...
// a slot manager should have operations like: occupySlot (aka setBit), freeSlot(aka clearBit), isSlotOccupied (aka isBitSet), findFirstFreeSlot (aka find first clear bit)...

#include<stdio.h>
#include<stdint.h>

void occupySlot(uint16_t *bitmap, int slot)
{
    if(slot < 0 || slot > 15)
    {
        printf("Invalid slot!");
        return;
    }

    uint16_t mask = (uint16_t)(1u << slot);
    *bitmap |= mask;

}

void freeSlot(uint16_t *bitmap, int slot)
{
    if(slot < 0 || slot > 15)
    {
        printf("Invalid slot!");
        return;
    }

    uint16_t mask = (uint16_t)~(1u << slot);
    *bitmap &= mask;

}

int isSlotOccupied(uint16_t bitmap, int slot)
{
    if(slot < 0 || slot > 15)
    {
        printf("Invalid slot!");
        return -1;
    }

    uint16_t mask = (uint16_t) (1u << slot);
    return (bitmap & mask) != 0;
    // return (bitmap >> slot) & 1 != 0; //the other way...

}

int findFirstFreeSlot(uint16_t bitmap)
{
    int i = 0;
    while(i < 16)
    {
        uint16_t temp = bitmap;
        uint16_t mask = (uint16_t) (1u << i);
        temp &= mask;
        if(temp == 0)
        {
            return i;
        }
        else
        {
            i++;
            continue;
        }
    }

    return -1;

}

//helper function to print binary form of the bitmap;
void printBinary(uint16_t bitmap)
{
    for(int i =15; i >=0; i--)
    {
        printf("%d", (bitmap >> i) & 1); //moves the ith bit all the way to 0th position and after & with 1 we get the exact bit it is;
        if(i%4 ==0)
        {
            printf(" ");
        }
    }
    printf("\n");

}

int main()
{
    uint16_t bitmap = 0; // 0000 0000 0000 0000

    occupySlot(&bitmap, 0); // 0000 0000 0000 0001
    occupySlot(&bitmap, 3); // 0000 0000 0000 1001
    occupySlot(&bitmap, 7); // 0000 0000 1000 1001
    occupySlot(&bitmap, 12); // 0001 0000 1000 1001
    occupySlot(&bitmap, 15); // 1001 0000 1000 1001

    printBinary(bitmap); // 1001 0000 1000 1001

    freeSlot(&bitmap, 7); // 1001 0000 0000 1001

    printBinary(bitmap); // 1001 0000 0000 1001

    printf("%d\n",findFirstFreeSlot(bitmap)); // 1

    return 0;
}
