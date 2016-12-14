#pragma once

/* create a node for a linked list */
#define NODE(x) struct { x *next; }
#define DNODE(x) struct { x * prev; x *next; }

/* the each_var must be defined before */
#define LIST_FOREACH(head, each_var) \
	for(; (each_var = head); head = head->next)
