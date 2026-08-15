#ifndef DB_H
#define DB_H

#include "db_types.h"

#include <stdio.h>
#include <stdint.h>

int init_db(char *dbName, Database *db, uint32_t recordSize);
int open_db(char *dbName, Database *db);
int close_db(Database *db);

#endif