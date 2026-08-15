#ifndef STORAGE_H
#define STORAGE_H

#include "db_types.h"

int write_page_to_disk(FILE *fp, Page *p, long offset);
int append_page_to_disk(FILE *fp, Page *p);
int read_page_from_disk(FILE *fp, Page *p,long offset);
int write_file_header(FILE *fp, FileHeader *header);
int read_file_header(FILE *fp, FileHeader *header);
int write_page_header(FILE *fp, PageHeader *header,long offset);
int read_page_header(FILE *fp, PageHeader *header,long offset);

#endif