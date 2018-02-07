SOURCES=$(wildcard ./src/*.c)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -pipe -O2 -Wno-unused-variable \
	-Wno-unused-function -std=gnu99 -DKDEBUG -I./include/
LDFLAGS=-ldl -lreadline
EXECUTABLE=karat
DISCARD=$(OBJECTS) $(EXECUTABLE) include/karat/opcodes.inc.h

# check and see if we have ccache
HAS_CCACHE := $(shell command -v ccache 2> /dev/null)
ifdef HAS_CCACHE
	CC := ccache $(CC)
else
	CC := $(CC)
endif

.PHONY: clean tst run install profile

all: opcodes vm modules

vm: $(EXECUTABLE)

opcodes:
	lua gen.lua > include/karat/opcodes.inc.h

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# compile all the modules that come with Karat
modules:
	$(MAKE) -C mod/

# create debugging information
profile:
	$(MAKE) clean CFLAGS='$(CFLAGS) -pg -O2'
	$(MAKE) CFLAGS='$(CFLAGS) -pg -O2'

install:
	install -s karat /usr/local/bin/karat
	cp karat.1 /usr/local/man/man1/

# run karat on a simple test suite
run:
	./$(EXECUTABLE)

tst:
	echo $(LS) test/*.k | col | xargs -n 1 ./$(EXECUTABLE)

clean:
	$(RM) $(DISCARD)
	$(MAKE) -C mod/ clean
