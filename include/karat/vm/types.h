#pragma once

#include<stdint.h>

#define prog_loc(prog, i) (&(prog)->bytes[i])
#define prog_size(prog) ((prog)->size)
#define prog_entry(prog) ((prog)->entry)

typedef struct prog_s {
    uint8_t *bytes;
    size_t entry;
    size_t size;
} *prog_t;

/* native karat types */
typedef uint8_t k8_t;
typedef uint16_t k16_t;
typedef uint32_t k32_t;
typedef uint64_t k64_t;
