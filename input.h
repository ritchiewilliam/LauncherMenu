#ifndef MENU_INPUT_H
#define MENU_INPUT_H

#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <errno.h>

typedef struct controller {
    struct libevdev* device;
    int rc;
} controller;

controller* init_input(const char * file);

#endif //MENU_INPUT_H




//void print_event (struct input_event *ev);