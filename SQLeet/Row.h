#pragma once
#define ROW_H

#define USERNAME_SIZE 32
#define EMAIL_SIZE 255

typedef struct {
	int id;
	char username[USERNAME_SIZE];
	char email[EMAIL_SIZE];
} Row;

void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);