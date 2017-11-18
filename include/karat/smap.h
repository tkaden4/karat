#pragma once

#include<wchar.h>
#include<karat/list.h>

#define HASH_BUCKETS (229 * 7 + 1)    /* not much thought, just a small prime number */

struct smap_node {
    SLINK(struct smap_node);
    wchar_t *key;
    void *value;
};

struct smap {
    void(*destroy_val_f)(void *);   /* so we can remove nodes dependent on what is being held */
    struct smap_node *map[HASH_BUCKETS];
};

struct smap *smap_create(void(*destroy_val)(void *));
void *smap_lookup(struct smap *map, const wchar_t *key);
void smap_insert(struct smap *map, const wchar_t *key, void *type);
void smap_destroy(struct smap *map);
