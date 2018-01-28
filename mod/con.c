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

//TRAPS();
//INTS();

struct con_data {
    struct termios old_attr;
    struct termios new_attr;
};

static int on_module_load(struct con_data **data)
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

static int on_module_unload(struct con_data *data)
{
    tcsetattr(fileno(stdin), TCSANOW, &data->old_attr);
    free(data);
    return 0;
}


