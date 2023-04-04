#include "gui.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

static Display* dpy;
static int scr;
static Window win;
static GC gc;

static Position * boxPositions;

int selected = 0;

void init() {
    dpy = XOpenDisplay(NULL);
    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, WIDTH, HEIGHT, 0, foreground, background);

    XGCValues mask;
    mask.line_width = 5;
    gc = XCreateGC(dpy, win, GCLineWidth, &mask);

    XMapWindow(dpy, win);
    XFlush(dpy);
    usleep(20000);
}

void keyboardLoop() {
//  Keyboard input was tested at first. Keeping for later implementation

    XEvent event;
    XSelectInput(dpy, win, KeyPressMask);

    while (1) {
        XNextEvent(dpy, &event);
        if(event.type == KeyPress) {

            KeySym keysym = XLookupKeysym(&event.xkey, 0);
//            printf("%lu\n", keysym);
            switch (keysym) {
                case XK_Right:
                    XSetForeground(dpy, gc, background);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], GAP_HL);
                    selected = (selected + 1) % 5;
                    XSetForeground(dpy, gc, 0x8c97ab);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], GAP_HL);

                    break;
                case XK_Left:
                    XSetForeground(dpy, gc, background);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], GAP_HL);
                    selected = ((selected - 1) + 5) % 5;
                    XSetForeground(dpy, gc, 0x8c97ab);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], GAP_HL);
                    break;
            }
            printf("selected: %d\n", selected);

        }
    }
}

void controllerLoop(Controller * dev) {
    struct input_event evdev; //Loops through all events in queue
    struct input_event prominentEvent; //Desired event pulled out of queue
    struct input_event pastEvent; //Previous event used for joystick control

    while (1) {
        if (dev->rc == LIBEVDEV_READ_STATUS_SYNC || dev->rc == LIBEVDEV_READ_STATUS_SUCCESS || dev->rc == -EAGAIN) {
            if (libevdev_has_event_pending(dev->device)) {
                //Reset the event grabbed from queue
                prominentEvent.type = 0;
                prominentEvent.value = 0;
                while (libevdev_has_event_pending(dev->device)) { //Will loop through all events queued
                    dev->rc = libevdev_next_event(dev->device,
                                                  LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, &evdev);
                    if (dev->rc == LIBEVDEV_READ_STATUS_SUCCESS) {
                        if (evdev.type == 1) { //Want to respond to button press
                            prominentEvent = evdev;
                            //Value of event on release is (!(0) = 1) meaning type will not change, will change when button is pressed (!(1) = 0)
                            //Need a better solution to fit variety of controllers
                            prominentEvent.type = !(prominentEvent.value);
                        } else if (evdev.type == 3) { //Want to respond to direction input
                            if (abs(evdev.value) >
                                abs(prominentEvent.value)) { //out of events queued grab the largest value found
                                prominentEvent = evdev;
                            }
                        }
                    }
                }
                switch (prominentEvent.type) {
                    case 1: //Button press
                        //printf("Code: %d Value: %d\n", pastEvent.code, pastEvent.value);
                        break;
                    case 3: //Directional Input
                        if (prominentEvent.code == ABS_X) { //If the code is 0 it is horizontal
                            //Move selector to new box
                            selected = selectBox(dpy, win, gc, prominentEvent.value, pastEvent.value, boxPositions,
                                                 selected, 22000);
//                            printf("selected: %d\n", selected);
                            //Past event prevents selected from being changed if the joystick was in a similar position when selected was changed
                            pastEvent = prominentEvent;
                        } else if (prominentEvent.code == ABS_HAT0X) {
                            selected = selectBox(dpy, win, gc, prominentEvent.value, pastEvent.value, boxPositions,
                                                 selected, 1);
                            //printf("selected: %d\n", selected);
                            pastEvent = prominentEvent;
                        }
                        break;
                }
            }
        }
        usleep(10000);
    }
}

int main() {

    init();
    boxPositions = drawSelections(dpy, win, gc);

    XSetForeground(dpy, gc, 0x8c97ab);
    highlightRoundedRectangle(dpy, win, gc, boxPositions[0], GAP_HL);
    XFlush(dpy);

    char ** controllers = configure_input();
    struct Controller* dev = (Controller*)malloc(sizeof(Controller));

    if(controllers == NULL) {
        printf("No controller connected, using keyboard\n");
        keyboardLoop();
    }
    else {
        init_input(dev, controllers[0]);
        controllerLoop(dev);
    }

    XUnmapWindow(dpy,win);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}


