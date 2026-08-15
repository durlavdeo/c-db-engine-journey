#include "db_types.h"
#include "db.h"
#include "page.h"
#include "storage.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int init_db(char *dbName, Database *db, uint32_t recordSize)
{
    FILE *fp = fopen(dbName, "w+b");
    if (fp == NULL)
    {
        printf("File error!");
        exit(1);
    }
    uint32_t pageCapacity = (PAGE_SIZE - sizeof(PageHeader)) / recordSize;

    FileHeader fh =
        {
            PAGE_SIZE,
            recordSize,
            0, pageCapacity, 0};

    memcpy(&db->header, &fh, sizeof(FileHeader));
    db->fp = fp;

    int res = write_file_header(db->fp, &db->header);
    if (res != 0)
    {
        printf("Error updating FileHeader!\n");
        return 1;
    }

    Page p;

    create_page(db, &p);
    append_page(db, &p);

    return 0;
}

int open_db(char *dbName, Database *db)
{
    FILE *fp = fopen(dbName, "r+b");

    if (fp == NULL)
    {
        printf("Error opening the file!");
        exit(1);
    }
    int res = read_file_header(db->fp, &db->header);
    if (res != 0)
    {
        printf("Error reading FileHeader!\n");
        return 1;
    }
    db->fp = fp;

    return 0;
}

int close_db(Database *db)
{
    if (db->fp == NULL)
    {
        printf("No file is opened!\n");
        return -1;
    }
    fclose(db->fp);
    return 0;
}