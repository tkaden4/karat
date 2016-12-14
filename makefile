SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -std=c11

EXECUTABLE=karat

ifeq ($(OS), Windows NT)
	EXECUTABLE = karat.exe
endif

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -o $@ $^

run:
	./$(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE)
	$(RM) *.o
