SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -std=c11
karat : $(OBJECTS)
	$(CC) -o $@ $^
clean : 
	rm -f $(OBJECTS) karat
