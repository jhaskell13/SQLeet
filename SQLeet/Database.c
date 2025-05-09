#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "database.h"

Database* new_database() {
	Database* db = (Database*)malloc(sizeof(Database));
	db->num_tables = 0;
	return db;
}

void free_database(Database* db) {
	for (int i = 0; i < db->num_tables; i++) {
		free_table(db->tables[i].data);
	}
	free(db);
}

TableDefinition* find_table(Database* db, const char* name) {
	for (int i = 0; i < db->num_tables; i++) {
		if (strcmp(db->tables[i].name, name) == 0) {
			return &db->tables[i];
		}
	}
	return NULL;
}

int create_table(Database* db, const char* name, char column_names[][DB_MAX_COL_NAME_LENGTH], int num_columns) {
	if (db->num_tables >= DB_MAX_TABLES) {
		return -1;
	}

	TableDefinition* def = &db->tables[db->num_tables++];
	strncpy_s(def->name, DB_MAX_TABLE_NAME_LENGTH, name, _TRUNCATE);
	def->num_columns = num_columns;

	for (int i = 0; i < num_columns; i++) {
		strncpy_s(def->column_names[i], DB_MAX_COL_NAME_LENGTH, column_names[i], _TRUNCATE);
	}

	def->data = new_table();
	return 0;
}