#include<stdlib.h>
#include<stdint.h>

/* for rendering support */
#include<SDL2/SDL.h>

#include<karat/mod.h>
#include<karat/vm/types.h>
#include<karat/vm/vm.h>

#define VERSION "0.0.1"

MODULE(
    "karat.vga",
    VERSION,
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

enum {
    DRAW_LINE = 0,
    DRAW_POINT,
};

struct vga_data {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

int on_trap(struct vga_data *data, k8_t num, struct vm *vm)
{
    (void) num;
    SDL_RenderClear(data->renderer);
    SDL_SetRenderDrawColor(data->renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
    switch(vm_reg(vm, 0)){
    case DRAW_LINE:
        SDL_RenderDrawLine(data->renderer, vm_reg(vm, 1), vm_reg(vm, 2), vm_reg(vm, 3), vm_reg(vm, 4));
        break;
    case DRAW_POINT:
        SDL_RenderDrawPoint(data->renderer, vm_reg(vm, 1), vm_reg(vm, 2));
        break;
    };
    SDL_RenderPresent(data->renderer);
    return 0;
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
    SDL_SetWindowTitle((*data)->window, "karat.vga " VERSION);
    return 0;
err:
    free(*data);
    return 1;
}

int on_module_unload(struct vga_data *data)
{
    if(data){
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
