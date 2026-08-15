#ifndef PAGE_H
#define PAGE_H

#include "db_types.h"
#include "db.h"

int create_page(Database *db, Page *p);
int append_page(Database *db, Page *p);
int write_page(Database *db, Page *p, uint32_t pageNum);
int read_page(Database *db, Page *p,uint32_t pageNum);


#endif