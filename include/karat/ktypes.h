#pragma once

#include<stdint.h>
#include<stdlib.h>

typedef uint16_t addr_t;    /* address datatype */

#define CALLBACK(ret, ...) \
    struct { ret(*cb)(void *, ##__VA_ARGS__); void *data; }

#define SET_CB(c, fp, d) { c.cb = fp; c.data = d; }
#define MAKE_CB(type, fp, d) (type){ fp, d }
#define RAW_CB(type, fp) MAKE_CB(type, fp, NULL)

#define CALL_CB(x, ...) x.cb(x.data, ##__VA_ARGS__)
