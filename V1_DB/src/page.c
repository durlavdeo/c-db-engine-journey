#include "page.h"
#include "storage.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int create_page(Database *db, Page *p)
{
    memset(p, 0, sizeof(Page));
    p->header.recordCount = 0;
    p->header.slotBitmap = 0;
    return 0;
}

int append_page(Database *db, Page *p)
{
    int result = append_page_to_disk(db->fp, p);
    if (result != 0)
    {
        printf("Error writing page!\n");
        return -1;
    }

    db->header.pageCount++;

    int res = write_file_header(db->fp, &db->header);
    if (res != 0)
    {
        printf("Error updating FileHeader!\n");
        return -1;
    }
    return 0;
}

int write_page(Database *db, Page *p, uint32_t pageNum)
{
    if (pageNum < 0 || pageNum > db->header.pageCount - 1)
    {
        printf("Invalid page number!");
        return -1;
    }

    long file_header_size = sizeof(FileHeader);
    long offset = file_header_size + (long)(pageNum * db->header.pageSize);

    int result = write_page_to_disk(db->fp, p, offset);
    if (result != 0)
    {
        printf("Error writing the page.");
        return -1;
    }

    return 0;
}

int read_page(Database *db, Page *p, uint32_t pageNum)
{
    if (pageNum < 0 || pageNum > db->header.pageCount - 1)
    {
        printf("Invalid page number!");
        return -1;
    }

    long file_header_size = sizeof(FileHeader);
    long offset = file_header_size + (long)(pageNum * db->header.pageSize);

    int result = read_page_from_disk(db->fp, p, offset);
    if (result != 0)
    {
        printf("Error reading the page.");
        return -1;
    }

    return 0;
}