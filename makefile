SOURCES=$(wildcard ./src/*.c)
SOURCES+=main.c
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -Wno-unused-function -O0 -std=gnu99 -DKDEBUG -I./include/
LIBS=-lSDL2
EXECUTABLE=karat
# check and see if we have ccache
HAS_CCACHE := $(shell command -v ccache 2> /dev/null)
ifdef HAS_CCACHE
	CC=ccache cc
else
	CC=cc
endif

ifeq ($(OS), Windows_NT)
	EXECUTABLE = karat.exe
	RM = del
endif

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

run:
	./$(EXECUTABLE) ./test/asm.kt

clean:
	$(RM) $(EXECUTABLE)
	$(RM) $(OBJECTS)
