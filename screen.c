#include"screen.h"

#include"log.h"

int screen_init(struct screen_state *state)
{
	err_on(!state, "screen state not allocated");
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		warn("Unable to initialize SDL2");
		SDL_Quit();
		return 1;
	}
	#define WPOS SDL_WINDOWPOS_UNDEFINED
	const size_t scr_size = SCREEN_SIZE * PIXEL_SIZE;
	SDL_Window *w = SDL_CreateWindow("", WPOS, WPOS, scr_size, scr_size, 0);
	#undef WPOS
	if(!w){
		warn("window was unable to be created [%s]", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *r = SDL_CreateRenderer(w, -1, 0);
	if(!r){
		warn("renderer was unable to be created [%s]", SDL_GetError());
		SDL_DestroyWindow(w);
		SDL_Quit();
		return 1;
	}
	SDL_Texture *t = SDL_CreateTexture(	r, SDL_PIXELFORMAT_RGBA8888, 
										SDL_TEXTUREACCESS_STATIC, 
										SCREEN_SIZE, SCREEN_SIZE);
	if(!t){
		warn("texture was unable to be created [%s]", SDL_GetError());
		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		SDL_Quit();
	}
	state->window = w;
	state->renderer = r;
	state->texture = t;
	memset(state->gfx, 0x00000000, sizeof(rgba_t) * SCREEN_SIZE * SCREEN_SIZE);
	return 0;
}

void screen_draw(struct screen_state *state)
{
	err_on(!state, "screen state not allocated");
	SDL_UpdateTexture(	state->texture, NULL, 
						state->gfx, SCREEN_SIZE * sizeof(u32));

	SDL_RenderClear(state->renderer);
	SDL_RenderCopy(state->renderer, state->texture, NULL, NULL);
	SDL_RenderPresent(state->renderer);
}

void screen_clear(struct screen_state *state)
{
	err_on(!state, "screen state not allocated");
	warn_on(!state->gfx, "gfx buffer not initialized");
	if(state->gfx){
		memset(state->gfx, 0lu, GFX_SIZE);
	}
}

void screen_set_pixel(struct screen_state *state, u32 x, u32 y, u32 color)
{
	err_on(!state, "screen state not allocated");
	state->gfx[y * SCREEN_SIZE + x] = color;
}

void screen_destroy(struct screen_state *state)
{
	warn_on(!state, "screen state not allocated");
	if(state){
		if(state->texture){
			SDL_DestroyTexture(state->texture);
		}
		warn_on(!state->renderer, "screen renderer not initialized")
		if(state->renderer){
			SDL_DestroyRenderer(state->renderer);
		}
		warn_on(!state->window, "screen window not initialized")
		if(state->window){
			SDL_DestroyWindow(state->window);
		}
		SDL_Quit();
	}
}
