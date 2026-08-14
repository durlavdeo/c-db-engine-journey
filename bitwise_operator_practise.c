#include<stdio.h>
#include<stdint.h>


//position -> index
void setBit(uint8_t *x, int pos)
{

    *x |= (1 << pos); // so the lhs creates a mask..suppose the position is 3 then the mask created looks like : 00001000 and then it is OR with the *x setting that postion...because in OR operation 1 | 0 = 1;
}

void clearBit(uint8_t *x, int pos)
{

    *x &= ~(1 << pos);// so the lhs creates a mask..suppose the position is 2 then the mask created looks like : 00000100 and then it is inverted..
    // so the resulting mask is : 11111011 which is then AND with the *x clearing the postions....
}

void toggleBit(uint8_t *x, int pos)
{

    *x ^= (1<<pos);//so the XOR operator results 1 if the bits are different...so we are masking with the bit word where the bit at the position is 1...suppose the bit in *x at the postion pos is 0..
    // then XOR with 1 yields 1 becuase differnt bits...if the bit at pos is 1 then XOR with 1 yields 0 because same bits;
}

int isBitSet(uint8_t x, int pos)
{
    x &= (1 << pos);
    if(x == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

int countSetBits(uint8_t x)
{
    int i = 0;
    int count = 0;
    while(i < 8)
    {
        uint8_t temp = x & (1<<i++);
        if(temp == 0)
        {
            continue;
        }
        else
        {
            count++;
        }
    }

    return count;

}

int findFirstFreeSlot(uint8_t x)
{
    int i = 0;
    while(i < 8)
    {
        uint8_t temp = x & (1<<i);
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


int main()
{
    uint8_t x = 0;

    setBit(&x, 2);
    printf("%u\n",x); //00000100
    setBit(&x, 0);
    printf("%u\n",x); //00000101

    toggleBit(&x,0); //00000100
    toggleBit(&x,0); //00000101

    printf("%d\n",isBitSet(x,2)); // 1
    printf("%d\n",isBitSet(x,1)); // 0

    printf("%d\n",countSetBits(x)); // 2

    printf("%d\n",findFirstFreeSlot(x)); // 1

    clearBit(&x,2);
    printf("%u\n",x);//00000001
    return 0;

}
