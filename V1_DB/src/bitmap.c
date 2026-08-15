#include "bitmap.h"

#include <stdint.h>
#include<stdio.h>

void set_bit(uint32_t *bitmap, uint32_t index)
{
    uint32_t mask = (1u << index);
    *bitmap |= mask;
}

void clear_bit(uint32_t *bitmap, uint32_t index){
    uint32_t mask = ~(1u << index);
    *bitmap &= mask;
}

int is_bit_set(uint32_t bitmap, uint32_t index)
{
    uint32_t mask = (bitmap >> index);
    return (mask & 1) !=0;

}

uint32_t find_first_clear_bit(uint32_t bitmap)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((bitmap & (1u << i)) == 0)
            return i;
    }

    return UINT32_MAX;
}