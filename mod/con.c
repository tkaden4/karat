#include<karat/mod.h>
#include<karat/vm/vm.h>
#include<karat/vm/types.h>

#include<stdio.h>

#include<termios.h>
#include<unistd.h>

MODULE(
    "karat.con",
    "0.0.1",
    "Kaden Thomas"
);


enum {
    PORT_READ = 0,
    PORT_WRITE,
    PORT_AVAIL,
    N_PORTS
};

PORTS(N_PORTS);

struct con_data {
    struct termios old_attr;
    struct termios new_attr;
};

int on_port_write(void *_, unsigned port, k32_t data)
{
    (void)_;
    switch(port){
    case PORT_WRITE:
        printf("%c", (char)data);
        break;
    default:
        return 1;
    }
    return 0;
}

int on_module_load(struct con_data **data)
{
    struct con_data *d = malloc(sizeof(struct con_data));
    tcgetattr(fileno(stdin), &d->old_attr);
    d->new_attr = d->old_attr;
    d->new_attr.c_cc[VTIME] = 0;
    d->new_attr.c_cc[VMIN] = 0;
    d->new_attr.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(fileno(stdin), TCSANOW, &d->new_attr);
    *data = d;
    return 0;
}

int on_module_unload(struct con_data *data)
{
    tcsetattr(fileno(stdin), TCSANOW, &data->old_attr);
    free(data);
    return 0;
}
