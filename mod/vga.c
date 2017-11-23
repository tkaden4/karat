#include<karat/ktypes.h>
#include<karat/mod.h>
/* #include<karat/interrupt.h> */

MODULE(
    "karat.vga",
    "Kaden Thomas"
);

struct soft_irq {

};

struct hard_irq {

};

struct io_port {

};

/* interrupts by the module
 * Mod -> Karat */
const struct hard_irq hard_irqs[] = {

};

/* interrupts by the cpu
 * Karat -> Mod */
const struct soft_irq soft_irqs[] = {

};

int on_module_load(void **data)
{
    (void) data;
    return 0;
}

int on_module_unload(void *data)
{
    (void) data;
    return 0;
}
