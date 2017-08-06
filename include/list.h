#pragma once

/* create a node for a linked list */
#define SLINK(type) struct { type *next; }
#define DLINK(type) struct { type * prev; type *next; }

/* the each_var must be defined before */
#define LIST_FOREACH(type, head, each_var) \
for(type *each_var = NULL, *___walk = head; \
        (each_var = ___walk); ___walk = ___walk->next)

#define LIST_FREELOOP(type, head, each_var) \
for(type *___walk = head, *each_var = NULL, *___next = NULL; \
    (each_var = ___walk) && ((___next = ___walk->next), 1); ___walk = ___next)

#define LIST_FIND(type, head, found, cmp) \
for(type *___walk = head; (found = ___walk) && !(cmp); ___walk = ___walk->next);
