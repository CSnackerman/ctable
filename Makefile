sources=main.c table.c
exe=test_table

all:
	gcc $(sources) -I. -o $(exe)

clean:
	rm -rf $(exe)