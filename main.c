#include <stdio.h>
#include <stdlib.h>

#include "table.h"

// define header fields.
#define ID 0
#define ADDRESS 1
#define VALUE 2

#define N_FIELDS 3

int main() {

	// null-terminated array of null-terminated char arrays
	char* header [N_FIELDS + 1]; // +1 accounts for last NULL ptr

	// set header values
	header [ID] = "id";
	header [ADDRESS] = "address";
	header [VALUE] = "value";
	header [N_FIELDS] = NULL;


	Table* table = createTable (header);
	TableItem* items = NULL;

	// build table
	int testVal = 100;
	char* testVal2 = "woohoo";
	float testVal3 = 77.7;

	// row 1
	items = (TableItem*) malloc (N_FIELDS * sizeof(TableItem));
	setValue_string (&items [ID], "testVal");
	setValue_address (&items [ADDRESS], &testVal);
	setValue_int (&items [VALUE], testVal);
	addRow (table, items);

	// row 2
	items = (TableItem*) malloc (N_FIELDS * sizeof(TableItem));
	setValue_string (&items [ID], "testVal2");
	setValue_address (&items [ADDRESS], &testVal2);
	setValue_string (&items [VALUE], testVal2);
	addRow (table, items);

	// row 3
	items = (TableItem*) malloc (N_FIELDS * sizeof(TableItem));
	setValue_string (&items [ID], "testVal3");
	setValue_address (&items [ADDRESS], &testVal3);
	setValue_float (&items [VALUE], testVal3);
	addRow (table, items);


	printTable (table);

	destroyTable (table);

	return 0;
}