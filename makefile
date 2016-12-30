SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -O3 -std=c99 -DKDEBUG \
		-I./libs/include/ -L./libs
LIBS=-lSDL2
EXECUTABLE=karat

ifeq ($(OS), Windows_NT)
	EXECUTABLE = karat.exe
	RM = del
endif

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

run:
	./$(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE)
	$(RM) *.o
