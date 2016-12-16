#pragma once

typedef uint16_t addr_t;	/* address datatype */


#define CALLBACK(ret, ...) \
struct { ret(*cb)(void *, ##__VA_ARGS__); void *data; }

#define RAW_CB(func) { func, NULL }

typedef CALLBACK(void, uint8_t) read_cb;
#define CALL_CB(x, ...) x.cb(x.data, ##__VA_ARGS__)
