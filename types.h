#pragma once

typedef uint16_t addr_t;	/* address datatype */
typedef uint8_t io_t;		/* io port datatype */

#define CALLBACK(ret, ...) \
struct { ret(*cb)(void *, ##__VA_ARGS__); void *data; }

#define SET_CB(cb, fp, data) { cb->cb = fp; cb->data = data; }
#define MAKE_CB(type, fp, data) (type){ fp; data; }

#define RAW_CB(func) { func, NULL }
typedef CALLBACK(void, uint8_t) read_cb;
typedef CALLBACK(uint8_t) write_cb;
#define CALL_CB(x, ...) x.cb(x.data, ##__VA_ARGS__)

#define U(size) \
typedef uint ## size ## _t u ## size

#define I(size) \
typedef int ## size ## _t i ## size


U(8);
U(16);
U(32);
U(64);

I(8);
I(16);
I(32);
I(64);

#undef U
#undef I
