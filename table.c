#include "table.h"

// helper functions
static void updateColumnWidths (Table* t);
static void setValue (TableItem* item, TableItemType type, int iVal, float fVal, char* sVal, void* aVal);
static void* convertStringToType (char* str, TableItemType type);


Table* createTable (char** header) {

	// locals
	int c;

	Table* t = (Table*) malloc (sizeof(Table));

	t->header = header;

	t->rows = 1;

	// accumulate number of columns from header
	for (t->cols = 0; header[t->cols] != NULL; t->cols++);

	// allocate 1 row
	t->cells = (TableItem**) malloc (sizeof(TableItem*));

	// allocate column array
	t->cells [0] = (TableItem*) malloc (t->cols * sizeof(TableItem));

	// copy header values into row 1
	for (c = 0; c < t->cols; c++) {

		t->cells [0][c].type = STRING;
		t->cells [0][c].row = 0;
		t->cells [0][c].col = c;

		strcpy (t->cells [0][c].value.str_val, header[c]);
	}

	// allocate columnWidths array
	t->colWidths = (int*) calloc (t->cols, sizeof(int));

	return t;
}

void addRow (Table* t, TableItem* items) {

	// set row and col of items
	int c;
	for (c = 0; c < t->cols; c++) {
		items->row = t->rows;
		items->col = c;
	}

	// reallocate a new row
	t->cells = (TableItem**) realloc (t->cells, ++t->rows * sizeof (TableItem*));

	// handoff
	t->cells [t->rows - 1] = items;
	items = NULL;
}

void destroyTable (Table* t) {

	if (t == NULL) {
		printf ("[ERROR] cannot destroy NULL table\n");
		return;
	}
	
	// free the column Widths array
	free (t->colWidths);

	// free the columns
	int r;
	for (r = 0; r < t->rows; r++) {
		free (t->cells[r]);
	}


	free (t->cells);
	free (t);

}

void printRow (Table* t, int row) {

	// locals
	int i, j, k;
	int cols = t->cols;
	int lastCol = t->cols - 1;
	int lastRow = t->rows - 1;
	int padding = 2;
	int columnWidth = 0;
	char itemStr [BUFFER_SIZE] = "";
	char row1 [1000] = "";
	char row2 [1000] = "";

	// build row1
	for (i = 0; i < t->cols; i++) {

		columnWidth = t->colWidths [i];
		
		strcat (row1, "+");

		for (j = 0; j < columnWidth + padding * 2; j++) {
			strcat (row1, "-");
		}

		if (i == lastCol) {
			strcat (row1, "+\n");
		}
	}

	// build row2	
	for (i = 0; i < t->cols; i++) {

		columnWidth = t->colWidths [i];

		itemToString (& t->cells [row][i], itemStr);
		int len = strlen (itemStr);

		strcat (row2, "|");

		// append left padding
		for (j = 0; j < padding + columnWidth - len; j++) {
			strcat(row2, " ");
		}

		// append item value
		strcat (row2, itemStr);

		// append right padding
		for (j = 0; j < padding; j++) {
			strcat (row2, " ");
		}

		// append horizontal end-cap
		if (i == lastCol) {
			strcat (row2, "|\n");
		}
	}
	
	if (row == 0 || row == 1) {
		printf ("%s", row1);
	}

	printf ("%s", row2);

	if (row == lastRow) {
		printf ("%s", row1);
	}
}

void printTable (Table* t) {
	
	int r, c;

	if (t == NULL) {
		printf ("[ERROR] cannot print NULL table\n");
		return;
	}

	int* col_widths = (int*) calloc (t->cols,  sizeof(int));
	updateColumnWidths (t);
	free(col_widths);

	for (r = 0; r < t->rows; r++) {
		printRow (t, r);
	}
}

void itemToString (TableItem* item, char* str) {

	if (item == NULL) {
		printf ("[ERROR] cannot convert NULL item to string\n");
		return;
	}

	char buffer [BUFFER_SIZE];

	switch (item->type) {

		case INTEGER:
			sprintf (buffer, "%d", item->value.int_val);
			break;

		case FLOAT:
			sprintf (buffer, "%.5f", item->value.float_val);
			break;

		case STRING:
			sprintf (buffer, "%s", item->value.str_val);
			break;

		case MEM_ADDR:
			sprintf (buffer, "%p", item->value.ptr_val);
	}

	strcpy (str, buffer);
}



void setValue_int (TableItem* item, int val) {
	setValue (item, INTEGER, val, 0, 0, 0);
}

void setValue_float (TableItem* item, float val) {
	setValue (item, FLOAT, 0, val, 0, 0);
}

void setValue_string (TableItem* item, char* val) {
	setValue (item, STRING, 0, 0, val, 0);
}

void setValue_address (TableItem* item, void* val) {
	setValue (item, MEM_ADDR, 0, 0, 0, val);
}


TableItem* getItemAt (Table* t, int r, int c) {
	return &t->cells [r][c];
}

void setValueAt_int (Table* t, int r, int c, int val) {
	TableItem* item = getItemAt (t, r, c);
	setValue_int (item, val);
}

void setValueAt_float (Table* t, int r, int c, float val) {
	TableItem* item = getItemAt (t, r, c);
	setValue_float (item, val);
}

void setValueAt_string (Table* t, int r, int c, char* val) {
	TableItem* item = getItemAt (t, r, c);
	setValue_string (item, val);
}

void setValueAt_address (Table* t, int r, int c, void* val) {
	TableItem* item = getItemAt (t, r, c);
	setValue_address (item, val);
}



// helpers

static void updateColumnWidths (Table* t) {

	// locals
	char buffer [BUFFER_SIZE];

	int longest;
	int r, c;
	
	for (c = 0; c < t->cols; c++) {

		longest = 0;

		for (r = 0; r < t->rows; r++) {

			itemToString (&t->cells[r][c], buffer);
			int len = strlen (buffer);

			if (len > longest) {
				longest = len;
			}
		}

		t->colWidths [c] = longest;
	}
}

static void setValue (TableItem* item, TableItemType type, int iVal, float fVal, char* sVal, void* aVal) {

	switch (type) {

		case INTEGER:
			item->type = type;
			item->value.int_val = iVal;
			return;
		
		case FLOAT:
			item->type = type;
			item->value.float_val = fVal;
			return;

		case STRING:
			item->type = type;
			strcpy (item->value.str_val, sVal);
			return;

		case MEM_ADDR:
			item->type = type;
			item->value.ptr_val = aVal;
			return;

	}
}