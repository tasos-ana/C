CC = gcc
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
TARGET = keylogger

# the build target executable:
all: .pre_clean .keylogger


# the single build target:
.keylogger: keylogger.c keylogger.h main.c 
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(TARGET).c
 
# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) $(TARGET) *.o *~
#-------------------------------------------------------------------------------

.pre_clean: 
	$(RM) $(TARGET) *.o *~
