#include "Table.h"
#include <stdlib.h>
#include <string.h>

Table* new_table() {
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    for (int i = 0; i < TABLE_MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }
    return table;
}

void* row_slot(Table* table, int row_num) {
    int page_num = row_num / ROWS_PER_PAGE;
    if (table->pages[page_num] == NULL) {
        table->pages[page_num] = malloc(PAGE_SIZE);
    }
    int row_offset = row_num % ROWS_PER_PAGE;
    return (char*)table->pages[page_num] + row_offset * ROW_SIZE;
}

void insert_row(Table* table, Row* row) {
    if (table->num_rows >= TABLE_MAX_ROWS) {
        printf("Overflow Error: Table has maximum rows.");
        return;
    }
    void* destination = row_slot(table, table->num_rows);
    serialize_row(row, destination);
    table->num_rows += 1;
}

void free_table(Table* table) {
    for (int i = 0; i < TABLE_MAX_PAGES; i++) {
        free(table->pages[i]);
    }
    free(table);
}