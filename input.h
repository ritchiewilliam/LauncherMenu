#ifndef MENU_INPUT_H
#define MENU_INPUT_H

#include <poll.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <errno.h>
#include "gui.h"

typedef struct Controller {
    struct libevdev* device;
    int joyMax;
    int joyMin;
    int A;
    int B;
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

int initInput(Controller * con, const char * file, int * fd);

char ** findInput(int * devices);

char * selectInput(Display * dpy, Window win, GC gc, char** deviceFiles, int n);

void menuLoop (Display* dpy, Window win, GC gc, Controller dev, int rc, struct pollfd* events);


#endif //MENU_INPUT_H




//void print_event (struct input_event *ev);
