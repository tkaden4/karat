MODULE_SRC=$(wildcard *.c)
MODULE_OBJ=$(MODULE_SRC:.c=.so)
CFLAGS=-Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-function -std=gnu99 -DKDEBUG -I../include/
LDFLAGS=-lSDL2

all: $(MODULE_OBJ)

%.so : %.c
	$(CC) -fPIC -shared  $^ -o $@ $(CFLAGS) $(LDFLAGS) -I../include/

clean:
	$(RM) $(MODULE_OBJ)
