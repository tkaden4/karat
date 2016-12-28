#include"screen.h"

#include"mem.h"
#include"log.h"

#define MODULE_NAME L"sdl_screen"

/* contains all state needed to run the module */
struct smod_data {
	struct screen_state *state;
	/* writeback buffer */
	struct {
		u8 base[64];
	} buff;
};

#define FROM_VPTR(x) ((struct smod_data *)(x))

INTERNAL void mod_init(void *data)
{
	err_on(!data, "module data not initialized");
	struct smod_data *smod = FROM_VPTR(data);
	smod->state = s_alloc(struct screen_state);
	screen_init(smod->state);
}


INTERNAL uint8_t mod_write(void *data)
{
	err_on(!data, "module not allocated");
	struct smod_data *smod = FROM_VPTR(data);
	return (u8)(intptr_t)smod;
}

INTERNAL void mod_destroy(void *data)
{
	warn_alloc(data);
	struct smod_data *smod = FROM_VPTR(data);
	warn_alloc(smod->state);

	screen_destroy(smod->state);

	s_free(smod->state);
	s_free(smod);
}

INTERNAL void on_clear(void *data, u8 *args, size_t nargs)
{
	(void) args;
	(void) nargs;
	warn_alloc(data);
	struct smod_data *smod = FROM_VPTR(data);
	warn_alloc(smod->state);
	screen_clear(smod->state);
	screen_draw(smod->state);
}

INTERNAL void on_flood(void *data, u8 *args, size_t nargs)
{
	(void) args;
	(void) nargs;
	warn_alloc(data);
	struct smod_data *smod = FROM_VPTR(data);
	warn_alloc(smod->state);
	screen_flood(smod->state, WHITE);
	screen_draw(smod->state);
}

struct mod *screen_module(struct mod *module)
{
	err_on(!module, "module not allocated");
	struct smod_data *smod = s_alloc(struct smod_data);
	zmem(*smod);
	struct io_table table;
	io_table_init(&table);
	io_table_add(&table, 0x80, 0, MAKE_CB(io_cb, on_clear, smod));
	io_table_add(&table, 0x81, 0, MAKE_CB(io_cb, on_flood, smod));
	return load_internal_module(module, 
								MODULE_FTABLE(
									 mod_init, 		smod,
									 mod_write, 	smod,
									 mod_destroy, 	smod
								),
								table,
								MODULE_NAME);
}

#undef FROM_VPTR
#undef MODULE_NAME
