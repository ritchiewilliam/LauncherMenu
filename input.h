#ifndef MENU_INPUT_H
#define MENU_INPUT_H

#include <libevdev-1.0/libevdev/libevdev.h>
#include <errno.h>

typedef struct Controller {
    struct libevdev* device;
    int rc;
} Controller;

typedef struct Input {
    char A;
    char B;
    char X;
    char Y;

    char joyLV;
    int joyLVBounds;
    char joyLH;
    int joyLHBounds;

    char dpadV;
    char dpadH;

    char bumpL;
    char bumpR;
} Input;

int init_input(Controller * con, const char * file);

char ** configure_input();

#endif //MENU_INPUT_H




//void print_event (struct input_event *ev);
