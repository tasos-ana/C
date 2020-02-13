all: list hashtable

symtablehash.o: symtable.h symtablehash.c
		gcc -c symtablehash.c

symtablelist.o: symtable.h symtablelist.c
		gcc -c symtablelist.c

list: symtable.h symtablelist.o
	gcc -Wall -ansi -pedantic -o testsymtabList testsymtab.c symtablelist.o

hashtable: symtable.h symtablehash.o
	  gcc -Wall -ansi -pedantic -o testsymtabHash testsymtab.c symtablehash.o
	  
clean:
	rm -rf *.o
