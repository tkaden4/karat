#pragma once

#include<wchar.h>
#include"list.h"

#define HASH_BUCKETS 937

struct smap_node {
	SLINK(struct smap_node);
	const wchar_t *key;
	void *value;
};

struct smap {
	struct smap_node *map[HASH_BUCKETS];
};

struct smap *smap_create();
void *smap_lookup(struct smap *map, const wchar_t *key);
void smap_insert(struct smap *map, const wchar_t *key, void *type);
void smap_destroy(struct smap *map);
