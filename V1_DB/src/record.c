#include "db_types.h"
#include "record.h"
#include "bitmap.h"
#include "page.h"
#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INVALID_RECORD_ID UINT32_MAX


uint32_t insert_record(Database *db, void *record)
{
init:
    // find the page with the free space...essentially finding the page number...
    uint32_t page_number = find_first_clear_bit(db->header.pageBitmap);
    if (page_number == UINT32_MAX)
    {
        printf("The database is full! Remove some records to add new one!");
        return INVALID_RECORD_ID;
    }

    // checking if the page exists for the found page number
    if (page_number >= db->header.pageCount) // we have page bitmap as 0000 0000 0000 0000..the zeros represent the page which are either non existent or empty...so for that we need to confirm the page number with the page count..also we are checking for ">=" becuase page number is simply an index starting from 0...
    {
        Page newPage;
        create_page(db, &newPage);
        append_page(db, &newPage);
    }

    Page p;
    // read that page into p;
    read_page(db, &p, page_number);

    // find the free slot in the page...
    int free_slot = find_first_clear_bit(p.header.slotBitmap);
    if (free_slot == -1)
    {
        printf("This page is full! Searching new page!");
        set_bit(&db->header.pageBitmap, page_number); // setting the bit to mark page as full...
        write_file_header(db->fp, &db->header);       // updating the file header to persist new information...
        goto init;                                    // research for new page with empty space....
    }

    void *slot = p.data + (free_slot * db->header.recordSize); // calculating the slots physical location inside the page

    memcpy(slot, record, db->header.recordSize); // copying the record into slot

    set_bit(&p.header.slotBitmap, free_slot); // setting the slot bit to mark it as occupied...

    p.header.recordCount++; // update record count in the page header

    write_page(db, &p, page_number); // write the page to the file...

    if (p.header.recordCount == db->header.pageCapacity) // checking if the page becomes full after writing this record..
    {
        set_bit(&db->header.pageBitmap, page_number); // if yes, then set the bit to mark as full...
        write_file_header(db->fp, &db->header);       // and update the file header to persist...
    }

    printf("Record inserted successfully!");
    uint32_t recordId = (page_number * db->header.pageCapacity) + free_slot; // a physical slot position in the file...

    return recordId;
}

int read_record(Database *db, void *buffer, uint32_t recordId)
{
    uint32_t pageNum = recordId / db->header.pageCapacity;

    uint32_t slotNum = recordId % db->header.pageCapacity;

    Page p;

    // read page
    if (read_page(db, &p, pageNum) != 0)
    {
        return -1;
    }

    // does the record actually exist?
    if (!is_bit_set(p.header.slotBitmap, slotNum))
    {
        return -1;
    }

    // locate record inside page
    void *slot = p.data + (slotNum * db->header.recordSize);

    memcpy(buffer, slot, db->header.recordSize);

    return 0;
}

int update_record(Database *db, uint32_t recordId, void *record)
{
    uint32_t pageNum = recordId / db->header.pageCapacity;

    uint32_t slotNum = recordId % db->header.pageCapacity;

    Page p;

    // read page...
    if (read_page(db, &p, pageNum) != 0)
    {
        return -1;
    }

    // does the record actually exist?
    if (!is_bit_set(p.header.slotBitmap, slotNum))
    {
        return -1;
    }

    // locate record inside page...
    void *slot = p.data + (slotNum * db->header.recordSize);

    memcpy(slot, record, db->header.recordSize); // replace the record...

    if (write_page(db, &p, pageNum) != 0) // persist the new updated record.
    {
        return -1;
    }

    return 0;
}

int delete_record(Database *db, uint32_t recordId)
{
    uint32_t pageNum = recordId / db->header.pageCapacity;

    uint32_t slotNum = recordId % db->header.pageCapacity;

    // does the page exist?
    if (pageNum >= db->header.pageCount)
    {
        return -1;
    }
    
    Page p;
    
    if (read_page(db, &p, pageNum) != 0)
    {
        return -1;
    }

    // does the record exist?
    if (!is_bit_set(p.header.slotBitmap, slotNum))
    {
        return -1;
    }

    // mark slot as free...
    clear_bit(&p.header.slotBitmap, slotNum);

    p.header.recordCount--;

    // if page was previously full, it now has free space....
    if (p.header.recordCount < db->header.pageCapacity)
    {
        clear_bit(&db->header.pageBitmap, pageNum);
    }

    // persist page
    if (write_page(db, &p, pageNum) != 0)
    {
        return -1;
    }

    // persist file header
    if (write_file_header(db->fp, &db->header) != 0)
    {
        return -1;
    }

    return 0;
}