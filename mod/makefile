MODULE_SRC=$(wildcard *.c)
MODULE_OBJ=$(MODULE_SRC:.c=.so)
CFLAGS=-Wall -Wextra -Werror -Wno-unused-function -std=gnu99 -DKDEBUG -I../include/
LDFLAGS=

all: $(MODULE_OBJ)

%.so : %.c
	$(CC) -fpic -shared  $^ -o $@ $(CFLAGS) $(LDFLAGS) -I../include/

clean:
	$(RM) $(MODULE_OBJ)
