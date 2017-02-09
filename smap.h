#pragma once

#include<wchar.h>
#include"list.h"

#define HASH_BUCKETS 937

typedef int(*smap_cmp)(const wchar_t *, const wchar_t *);

struct smap_node {
	SLINK(struct smap_node);
	const wchar_t *key;
	void *value;
};

struct smap {
	smap_cmp cmp;
	struct smap_node *map[HASH_BUCKETS];
};

struct smap *smap_create(smap_cmp cmp);
void *smap_lookup(struct smap *map, const wchar_t *key);
void smap_insert(struct smap *map, const wchar_t *key, void *type);
void smap_destroy(struct smap *map);
