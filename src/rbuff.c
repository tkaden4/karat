#include<parse/rbuff.h>
#include<alloc.h>

void rbuff_init(struct rbuff *buff, size_t cap)
{
	buff->buff = s_calloc(cap, sizeof(struct rbuff_node *));
	buff->size = 0;
	buff->start = 0;
	buff->cap = cap;
}

/* get the actual index of the nth element in the buffer */
static inline size_t rbuff_elem_index(const struct rbuff *buff, size_t n)
{
	return (buff->start + n) % buff->cap;
}

struct rbuff_node *rbuff_elem(struct rbuff *buff, size_t n)
{
	if(n > buff->cap){
		return NULL;
	}
	return buff->buff[rbuff_elem_index(buff, n)];
}

void rbuff_push_back(struct rbuff *buff, struct rbuff_node *node)
{
	buff->buff[rbuff_elem_index(buff, buff->size)] = node;
	++buff->size;
}

void rbuff_push_front(struct rbuff *buff, struct rbuff_node *node)
{
	buff->start = rbuff_elem_index(buff, buff->cap);
	buff->buff[buff->start] = node;
	++buff->size;

}

struct rbuff_node *rbuff_pop_back(struct rbuff *buff)
{
	//struct rbuff_node *ret = buff->buff[];
	--buff->size;
	//return ret;
	return NULL;
}

struct rbuff_node *rbuff_pop_front(struct rbuff *buff)
{
	struct rbuff_node *ret = buff->buff[buff->start];
	buff->start = rbuff_elem_index(buff, 1);
	--buff->size;
	return ret;
}

void rbuff_destroy(struct rbuff *buff)
{
	s_free(buff->buff);
}
