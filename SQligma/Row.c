#include "Row.h";
#include <string.h>

void serialize_row(Row* source, void* destination) {
	memcpy(destination, source, sizeof(Row));
}

void deserialize_row(void* source, Row* destination) {
	memcpy(destination, source, sizeof(Row));
}