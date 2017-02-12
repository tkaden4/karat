#pragma once

#include<wchar.h>
#include"list.h"

#define HASH_BUCKETS 229

struct smap_node {
	SLINK(struct smap_node);
	wchar_t *key;
	void *value;
};

struct smap {
	void(*destroy_node_f)(void *);
	struct smap_node *map[HASH_BUCKETS];
};

struct smap *smap_create(void(*destroy_node)(void *));
struct smap *smap_create_d();	/* create with default destroy function (free) */
void *smap_lookup(struct smap *map, const wchar_t *key);
void smap_insert(struct smap *map, const wchar_t *key, void *type);
void smap_destroy(struct smap *map);
