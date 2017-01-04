#pragma once

#include<SDL2/SDL.h> /* this will have to be compiled with sdl2 */
#include<stdbool.h>

#include"module.h"
#include"types.h"
#include"bits.h"

#define WINDOW_SIZE 384
#define SCREEN_SIZE 64
#if WINDOW_SIZE % SCREEN_SIZE != 0
#error "Invalid window and screen sizes, window % screen"
#endif
#define PIXEL_SIZE (WINDOW_SIZE/SCREEN_SIZE)
#define GFX_SIZE (SCREEN_SIZE * SCREEN_SIZE)

enum {
	FLAT =	0x000000FF,
	RED = 	0xFF000000 | FLAT,
	GREEN = 0x00FF0000 | FLAT,
	BLUE = 	0x0000FF00 | FLAT,
	CLEAR = 0x00000000,
	WHITE = RED | GREEN | BLUE | FLAT,
	BLACK = ~WHITE | FLAT
};
/* create a color from four integers */
#define I(f) ((u8)(f)) 
#define rgba(r, g, b, a) ( shl(I(r), 3) | shl(I(g), 2)| shl(I(b), 1) | I(a))
/* create a color from four floats */
#define frgba(r, g, b, a) rgba((r) * 0xFF, (g) * 0xFF, (b) * 0xFF, (a) * 0xFF)

#define rgb(r, g, b) rgba(r, g, b, 0xFF)
#define frgb(r, g, b) frgba(r, g, b, 1.f)

typedef u32 rgba_t;

struct screen_state {
	/* representation of pixels on screen */
	rgba_t gfx[GFX_SIZE];
	/* window to draw to */
	SDL_Window *window;
	/* texture to be drawn to screen */
	SDL_Texture *texture;
	/* context for manipulating pixels */
	SDL_Renderer *renderer;
};

static inline void screen_dims(
	u16 *win_width, u16 *win_height,
	u16 *scr_width, u16 *scr_height)
{
#define SETP(p, v) if(p){ *p = v; }
	SETP(win_width, WINDOW_SIZE);
	SETP(win_height, WINDOW_SIZE);
	SETP(scr_width, SCREEN_SIZE);
	SETP(scr_height, SCREEN_SIZE);
#undef SETP
}

struct mod *screen_module(struct mod *m);

int screen_init(struct screen_state *state);
void screen_draw(struct screen_state *state);
void screen_clear(struct screen_state *state);
void screen_flood(struct screen_state *state, rgba_t color);
void screen_set_pixel(struct screen_state *state, u32 x, u32 y, u32 color);
void screen_destroy(struct screen_state *state);
