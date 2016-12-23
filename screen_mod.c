#include"screen.h"

#include"mem.h"
#include"log.h"

#define MODULE_NAME L"sdl_screen"

/* what mode are we in? */
typedef u8 io_mode;
static const u8 io_reading = 0x00;
static const u8 io_writing = 0x01;

/* contains all state needed to run the module */
struct smod_data {
	struct screen_state *state;
	io_mode iom;
};

#define FROM_VPTR(x) ((struct smod_data *)x)

INTERNAL void mod_init(void *data)
{
	err_on(!data, "module data not initialized");
	struct smod_data *smod = FROM_VPTR(data);
	smod->state = s_alloc(struct screen_state);
	zstruct(*smod->state);
	screen_init(smod->state);
	smod->iom = io_reading;
}

INTERNAL void mod_read(void *data, uint8_t byte)
{
	struct smod_data *smod = FROM_VPTR(data);
	if(smod->iom != io_reading){
		debug("expected a write, ignoring and switching to read");
		smod->iom = io_reading;
	}
	debug("\"%ls\" : in 0x%X", MODULE_NAME, byte);
}

/* TODO actually implement */
INTERNAL uint8_t mod_write(void *data)
{
	return (intptr_t)data & 0xFF;
}

INTERNAL void mod_destroy(void *data)
{
	warn_on(!data, "module data not initialized, unable to destroy");
	if(!data){
		return;
	}
	struct smod_data *smod = FROM_VPTR(data);
	warn_on(!data, "screen not allocated");
	if(smod){
		screen_destroy(smod->state);
		s_free(smod->state);
		s_free(smod);
	}
}

struct mod *screen_module(struct mod *module)
{
	err_on(!module, "module not allocated");
	struct smod_data *smod = s_alloc(struct smod_data);
	zstruct(*smod);
	return load_internal_module(module, 
								MODULE_FTABLE(
									 mod_init, 		smod,
									 mod_read, 		smod,
									 mod_write, 	smod,
									 mod_destroy, 	smod
								),
								MODULE_NAME);
}

#undef FROM_VPTR
#undef MODULE_NAME
