#pragma once

#include<stddef.h>

struct rbuff_node {};

#define rbuff_entry(rbnode, parent_type, varname)\
	((parent_type *)((void *)(rbnode) - offsetof(parent_type, varname)))

#define rbuff_for_each(buffp, ptype, varname, subname)\
	ptype *varname = NULL;\
	for(size_t ___indexer = (buffp)->start; \
			__indexer < (buffp)->size && (varname = rbuff_entry(buffp, ptype, subname)); \
			___indexer += ((__indexer + 1) % (buffp)->cap))

/* TODO make growable */
struct rbuff {
	struct rbuff_node **buff;
	size_t start;	/* index members start at */
	size_t size;	/* current amount of members */
	size_t cap;		/* current buffer capacity */
};

void rbuff_init(struct rbuff *buff, size_t cap);
struct rbuff_node *rbuff_elem(struct rbuff *buff, size_t n);
void rbuff_push_back(struct rbuff *buff, struct rbuff_node *node);
void rbuff_push_front(struct rbuff *buff, struct rbuff_node *node);
struct rbuff_node *rbuff_pop_front(struct rbuff *buff);
struct rbuff_node *rbuff_pop_back(struct rbuff *buff);
void rbuff_destroy(struct rbuff *buff);
