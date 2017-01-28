#pragma once

/* create a node for a linked list */
#define SLINK(x) struct { x *next; }
#define DLINK(x) struct { x * prev; x *next; }

/* the each_var must be defined before */
#define LIST_FOREACH(type, head, each_var) \
for(type *each_var = NULL, *walk = head; (each_var = walk); walk = walk->next)

#define LIST_FREELOOP(type, head, each_var, block) \
for(type *walk = head, *each_var = NULL, *next = NULL; \
	(each_var = walk); walk = next){ next = walk->next; block }

#define LIST_FIND(type, head, found, cmp) \
for(type *walk = head; (found = walk) && !(cmp); walk = walk->next);
