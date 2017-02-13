SOURCES=$(wildcard ./src/*.c)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -O0 -std=gnu99 -DKDEBUG \
		-I./include/
LIBS=-lSDL2
EXECUTABLE=karat

CC=ccache cc

ifeq ($(OS), Windows_NT)
	EXECUTABLE = karat.exe
	RM = del
endif

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

run:
	./$(EXECUTABLE)

test:
	valgrind ./$(EXECUTABLE) ./test/asm.kt

clean:
	$(RM) $(EXECUTABLE)
	$(RM) ./src/*.o
