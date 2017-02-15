#pragma once

/* create a node for a linked list */
#define SLINK(type) struct { type *next; }
#define DLINK(type) struct { type * prev; type *next; }

/* the each_var must be defined before */
#define LIST_FOREACH(type, head, each_var) \
for(type *each_var = NULL, *walk = head; (each_var = walk); walk = walk->next)

#define LIST_FREELOOP(type, head, each_var) \
for(type *walk = head, *each_var = NULL, *next = NULL; \
	(each_var = walk) && ((next = walk->next) || 1); walk = next)

#define LIST_FIND(type, head, found, cmp) \
for(type *walk = head; (found = walk) && !(cmp); walk = walk->next);
