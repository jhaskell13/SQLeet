#pragma once
#define TABLE_H

#include "Row.h"

#define PAGE_SIZE 4096
#define ROW_SIZE sizeof(Row)
#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_PAGES 100
#define TABLE_MAX_ROWS (ROWS_PER_PAGE * TABLE_MAX_PAGES)
#define TABLE_MAX_COLS 32

typedef struct {
    void* pages[TABLE_MAX_PAGES];
    int num_rows;
} Table;

Table* new_table();
void* row_slot(Table* table, int row_num);
void insert_row(Table* table, Row* row);
void free_table(Table* table);

typedef struct {
    char name[32];
    int num_columns;
    char column_names[TABLE_MAX_COLS][32];
    Table* data;
} TableDefinition;