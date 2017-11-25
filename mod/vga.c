#include<stdlib.h>
#include<stdint.h>

#include<karat/ktypes.h>
#include<karat/mod.h>
#include<karat/vm/types.h>

MODULE(
    "karat.vga",
    "Kaden Thomas"
);

#define alloc(type) (malloc(sizeof(type)))

#define VGA_WIDTH 800
#define VGA_HEIGHT 600

struct vga_data {
    /* TODO or is this in main memory? */
    k32_t *bitmap;
};

/* TODO byte-order may become an issue */
/* TODO handle transparency bits */
static void set_pix_s(struct vga_data *data, k16_t x, k16_t y, k32_t color)
{
    data->bitmap[(VGA_WIDTH * y) % (VGA_WIDTH * VGA_HEIGHT) + (x % VGA_WIDTH)] = color;
}

static void draw_rect(struct vga_data *data, k16_t x1, k16_t y1, k16_t x2, k16_t y2, k32_t color)
{
    for(register size_t i = y1; i < y2; ++i){
        for(register size_t k = x1; k < x2; ++k){
            set_pix_s(data, k, i, color);
        }
    }
}

static int on_module_load(struct vga_data **data)
{
    *data = alloc(struct vga_data);
    (*data)->bitmap = alloc(uint32_t[VGA_WIDTH * VGA_HEIGHT]);
    return 0;
}

static int on_module_unload(struct vga_data *data)
{
    if(data){
        if(data->bitmap){
            free(data->bitmap);
        }
        free(data);
    }
    return 0;
}
