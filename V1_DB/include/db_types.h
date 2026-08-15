#ifndef DB_TYPES_H
#define DB_TYPES_H

#include <stdio.h>
#include <stdint.h>

#define PAGE_SIZE 256

// =======================================DB Format=======================================
typedef struct
{
    uint32_t pageSize;
    uint32_t recordSize;
    uint32_t pageCount;
    uint32_t pageCapacity;
    uint32_t pageBitmap;

} FileHeader;

typedef struct
{
    uint32_t recordCount;
    uint32_t slotBitmap;
} PageHeader;

typedef struct
{
    PageHeader header;
    char data[PAGE_SIZE - sizeof(PageHeader)];
} Page;

typedef struct
{
    FILE *fp;
    FileHeader header;
} Database;

#endif