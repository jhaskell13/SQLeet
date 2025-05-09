#pragma once
#ifndef DATABASE_H
#define DATABASE_H

#include "Table.h"

#define DB_MAX_TABLES 10
#define DB_MAX_COLS 8
#define DB_MAX_COL_NAME_LENGTH 32
#define DB_MAX_TABLE_NAME_LENGTH 32

typedef struct {
	TableDefinition tables[DB_MAX_TABLES];
	int num_tables;
} Database;

Database* new_database();
void free_database(Database* db);

TableDefinition* find_table(Database* db, const char* name);
int create_table(Database* db, const char* name, char column_names[][DB_MAX_COL_NAME_LENGTH], int num_columns);

#endif