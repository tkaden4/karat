SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -std=c11

EXECUTABLE=karat
DEL = rm -f

ifeq ($(OS), Windows NT)
	DEL = del
	EXECUTBLE = karat.exe
endif

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -o $@ $^

run:
	./$(EXECUTABLE)


clean:
	$(DEL) $(EXECUTABLE)
	$(DEL) *.o
