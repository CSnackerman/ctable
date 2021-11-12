#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 255

enum TableItemType {
	INTEGER,
	FLOAT,
	STRING,
	MEM_ADDR
};
typedef enum TableItemType TableItemType;


union TableItemValue {
	int int_val;
	float float_val;
	char str_val [BUFFER_SIZE];
	void* ptr_val;
};
typedef union TableItemValue TableItemValue;


struct TableItem {
	TableItemValue value;
	TableItemType type;
	int row, col;
};
typedef struct TableItem TableItem;


struct Table {
	int rows, cols;
	int* colWidths;
	char** header;
	TableItem** cells;
};
typedef struct Table Table;

Table* createTable (char** header);
void addRow (Table* t, TableItem* items);
void destroyTable (Table* t);

void printRow (Table* t, int row);
void printTable (Table* t);

void setValue_int (TableItem* item, int val);
void setValue_float (TableItem* item, float val);
void setValue_string (TableItem* item, char* val);
void setValue_address (TableItem* item, void* val);

TableItem* getItemAt (Table* t, int r, int c);
void setValueAt_int (Table* t, int r, int c, int val);
void setValueAt_float (Table* t, int r, int c, float val);
void setValueAt_string (Table* t, int r, int c, char* val);
void setValueAt_address (Table* t, int r, int c, void* val);

void itemToString (TableItem* item, char* str);

#endif