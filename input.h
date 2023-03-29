//#ifndef X11_INPUT_H
//#define X11_INPUT_H
//
//#endif //X11_INPUT_H
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <errno.h>

typedef struct controller {
    struct libevdev* device;
    int rc;
} controller;

controller* init_input(const char * file);

//void print_event (struct input_event *ev);