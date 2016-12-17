#include"module.h"

#include"mem.h"

INTERNAL struct mod *create_module( read_cb update_cb, 
									addr_t addr, 
								  	const wchar_t *name)
{
	struct mod *ret = s_malloc(sizeof(struct mod));
	ret->name = name;
	ret->addr = addr;
	ret->on_read = update_cb;
	ret->next = NULL;
	return ret;
}

API void modules_init(struct modlist *list)
{
	/* TODO error handling on NULL */
	if(list){
		list->size = 0;
		list->head = NULL;
		list->tail = NULL;
	}
}

API struct mod *load_internal_module(	struct modlist *list,
										read_cb update_cb,
										addr_t addr,
										const wchar_t *name)
{
	if(list){
		struct mod *add_mod = create_module(update_cb, addr, name);
		if(!list->head){
			list->head = add_mod;
			list->tail = list->head;
		}else{
			list->tail->next = add_mod;
			list->tail = list->tail->next;
		}
		++list->size;
		return add_mod;
	}
	return NULL;
}

API void update_module(struct mod *m, uint8_t data)
{
	if(m){
		CALL_CB(m->on_read, data);
	}
}

API void modules_destroy(struct modlist *list) 
{
	if(list){
		for(struct mod *walk = list->head, *next; walk; walk = next){
			next = walk->next;
			s_free(walk);
		}
	}
}
