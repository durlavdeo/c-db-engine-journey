#ifndef SLOT_H
#define SLOT_H

#include <stdint.h>

void set_bit(uint32_t *bitmap, uint32_t index);
void clear_bit(uint32_t *bitmap, uint32_t index);
int is_bit_set(uint32_t bitmap, uint32_t index);
uint32_t find_first_clear_bit(uint32_t bitmap);

#endif