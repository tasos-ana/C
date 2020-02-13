all: myLibrary
	
myLibrary: myLibrary.c logfile.c originalFunctions.c
	gcc -Wall -fPIC -shared -o myLibrary.so myLibrary.c logfile.c originalFunctions.c -ldl -lcrypto

clean:
	rm -rf *.o myLibrary.so
