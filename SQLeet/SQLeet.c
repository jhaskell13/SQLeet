#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "row.h"
#include "table.h"
#include "database.h"

#define INPUT_BUFFER_SIZE 1024

int main() {
    char input[INPUT_BUFFER_SIZE];
    Database* db = new_database();

    while (1) {
        printf("db > ");
        if (!fgets(input, INPUT_BUFFER_SIZE, stdin)) {
            printf("Error reading input.\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';  // Strip newline

        // .exit
        if (strncmp(input, ".exit", 5) == 0) {
            free_database(db);
            exit(0);
        }

        // CREATE TABLE
        if (strncmp(input, "CREATE TABLE", 12) == 0) {
            char table_name[32];
            char columns_raw[256];

            int parsed = sscanf_s(input, "CREATE TABLE %31s (%255[^)])",
                table_name, (unsigned)_countof(table_name),
                columns_raw, (unsigned)_countof(columns_raw));
            if (parsed != 2) {
                printf("Syntax error. Usage: CREATE TABLE name (col1 col2 col3)\n");
                continue;
            }

            char column_names[DB_MAX_COLS][DB_MAX_COL_NAME_LENGTH];
            int num_columns = 0;

            char* context = NULL;
            char* token = strtok_s(columns_raw, " ", &context);
            while (token && num_columns < DB_MAX_COLS) {
                strncpy_s(column_names[num_columns++], DB_MAX_COL_NAME_LENGTH, token, _TRUNCATE);
                token = strtok_s(NULL, " ", &context);
            }

            if (create_table(db, table_name, column_names, num_columns) == 0) {
                printf("Table '%s' created with %d columns.\n", table_name, num_columns);
            }
            else {
                printf("Failed to create table (too many tables or name conflict).\n");
            }
            continue;
        }

        // INSERT INTO
        if (strncmp(input, "INSERT INTO", 11) == 0) {
            char table_name[32];
            int id;
            char username[32];
            char email[255];

            int parsed = sscanf_s(
                input,
                "INSERT INTO %31s VALUES (%d %31s %254s)",
                table_name, (unsigned)_countof(table_name),
                &id,
                username, (unsigned)_countof(username),
                email, (unsigned)_countof(email)
            );

            if (parsed != 4) {
                printf("Syntax error. Usage: INSERT INTO table VALUES (id username email)\n");
                continue;
            }

            TableDefinition* def = find_table(db, table_name);
            if (!def) {
                printf("Table '%s' not found.\n", table_name);
                continue;
            }

            Row row;
            row.id = id;
            strncpy_s(row.username, sizeof(row.username), username, _TRUNCATE);
            strncpy_s(row.email, sizeof(row.email), email, _TRUNCATE);
            insert_row(def->data, &row);

            printf("Inserted into %s.\n", table_name);
            continue;
        }

        // SELECT * FROM
        if (strncmp(input, "SELECT * FROM", 13) == 0) {
            char table_name[32];
            if (sscanf_s(input, "SELECT * FROM %31s", table_name, (unsigned)_countof(table_name)) != 1) {
                printf("Syntax error. Usage: SELECT * FROM table_name\n");
                continue;
            }

            TableDefinition* def = find_table(db, table_name);
            if (!def) {
                printf("Table '%s' not found.\n", table_name);
                continue;
            }

            for (int i = 0; i < def->data->num_rows; i++) {
                Row row;
                deserialize_row(row_slot(def->data, i), &row);
                printf("(%d, %s, %s)\n", row.id, row.username, row.email);
            }
            continue;
        }

        printf("Unrecognized command.\n");
    }
}
