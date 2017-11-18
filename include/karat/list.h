#pragma once

/* create a node for a linked list */
#define SLINK(type) struct { type *next; }
#define DLINK(type) struct { type * prev; type *next; }

/* the each_var must be defined before */
#define LIST_FOREACH( head, each_var) \
for(typeof(*head) *each_var = NULL, *___walk = head; \
        (each_var = ___walk); ___walk = ___walk->next)

#define LIST_FREELOOP(head, each_var) \
for(typeof(*head) *___walk = head, *each_var = NULL, *___next = NULL; \
    (each_var = ___walk) && ((___next = ___walk->next), 1); ___walk = ___next)

#define LIST_FIND(head, found, cmp) \
for(typeof(*head) *___walk = head; (found = ___walk) && !(cmp); ___walk = ___walk->next);
