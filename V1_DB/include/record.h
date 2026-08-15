#ifndef RECORD_H
#define RECORD_H

#include "db_types.h"

#include<stdio.h>
#include<stdint.h>
#include<string.h>


uint32_t insert_record(Database *db, void *record);
int read_record(Database *db, void *buffer, uint32_t recordId);
int update_record(Database *db, uint32_t recordId, void *record);
int delete_record(Database *db, uint32_t recordId);
// void find_record(Database *db);
// void count_records();


#endif