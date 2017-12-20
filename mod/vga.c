#include<stdlib.h>
#include<stdint.h>

/* for rendering support */
#include<SDL2/SDL.h>

#include<karat/mod.h>
#include<karat/vm/types.h>

MODULE(
    "karat.vga",
    "Kaden Thomas"
);

#define alloc(type) (calloc(1, sizeof(type)))

#define VGA_WIDTH 640
#define VGA_HEIGHT 480

#define R(color) ((color) & 0xf000)
#define G(color) ((color) & 0x0f00)
#define B(color) ((color) & 0x00f0)
#define A(color) ((color) & 0x000f)

/* TODO byte-order may become an issue */
/* TODO handle transparency bits */

#define sdl_error(fmt, ...) \
    SDL_Log(fmt ": %s", ##__VA_ARGS__, SDL_GetError())

typedef k16_t color_t;

struct vga_data {
    /* vga memory, TODO should it be in main memory? */
    color_t *bitmap;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

static inline void set_pix_s(struct vga_data *data, k16_t x, k16_t y, color_t color)
{
    data->bitmap[(VGA_WIDTH * y) % (VGA_WIDTH * VGA_HEIGHT) + (x % VGA_WIDTH)] = color;
}

static inline void draw_rect(struct vga_data *data, k16_t x1, k16_t y1, k16_t x2, k16_t y2, color_t color)
{
    for(register uint_fast16_t i = y1; i < y2; ++i){
        for(register uint_fast16_t k = x1; k < x2; ++k){
            set_pix_s(data, k, i, color);
        }
    }
}

int on_module_load(struct vga_data **data)
{
    if(SDL_Init(SDL_INIT_VIDEO)){
        sdl_error("Unable to initialize SDL2");
        return 1;
    }
    *data = alloc(struct vga_data);
    if(SDL_CreateWindowAndRenderer(VGA_WIDTH, VGA_HEIGHT, 0, &(*data)->window, &(*data)->renderer)){
        sdl_error("Unable to create window or renderer");
        goto err;
    }
    SDL_SetWindowTitle((*data)->window, "karat.vga window");
    (*data)->bitmap = alloc(color_t[VGA_WIDTH * VGA_HEIGHT]);
    return 0;
err:
    free(*data);
    return 1;
}

int on_module_unload(struct vga_data *data)
{
    if(data){
        if(data->bitmap){
            free(data->bitmap);
        }
        free(data);
    }
    if(data->renderer){
        SDL_DestroyRenderer(data->renderer);
    }
    if(data->window){
        SDL_DestroyWindow(data->window);
    }
    SDL_Quit();
    return 0;
}
