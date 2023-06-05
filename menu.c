#include "menu.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <X11/xpm.h>

Position * boxPositions;
int selected = 0;

int gap_hl = 0;

void drawSelections (Display * dpy, Window win, GC gc, int width, int height) {

    int boxWidth = (int)(((float)width)/6.4);
    int boxHeight = (int)(((float)height)/2.2);
    int gap = boxWidth + (int)(((float)width)/25.6);
    int alt = 1;
    int x = (width / 2) - (boxWidth / 2);
    int r = width / 96;

    XSetForeground(dpy, gc, FOREGROUND);

    int index = BOXES / 2;

    if(boxPositions == NULL) {
        boxPositions = (Position *)malloc(sizeof(Position) * BOXES);
    }


    for(int i = 0; i < BOXES; i++) {
        alt = -alt;
        x += gap * alt * i;

        index += i * alt;

        setPosition(&boxPositions[index], x, 200, boxWidth, boxHeight, r);

        //XFillRectangle(dpy, win, gc, x, 200, boxWidth, boxHeight);
        XFillRoundedRectangle(dpy, win, gc, boxPositions[index]);
//        printf("Location: %d to %d\n\n", x, x + boxWidth);
    }
    XFlush(dpy);
}

int selectBox (Display * dpy, Window win, GC gc, int promVal, int pastVal, int threshold) {

    gap_hl = WIDTH/192;



    //If the value pulled from the queue is past the threshold, and the previous value isn't in the same range, change can become 1 or -1
    int change = ((promVal >= threshold && pastVal <= threshold) - (promVal <= -threshold && pastVal >= -threshold));



    if(abs(change)) {
        XSetForeground(dpy, gc, BACKGROUND);
        highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);

        selected = (selected + change + BOXES) % BOXES;

        XSetForeground(dpy, gc, HIGHLIGHT);
        highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);

        XFlush(dpy);
    }
    return selected;
}

void keyboardLoop(Display * dpy, Window win, GC gc) {
//  Keyboard input was tested at first. Keeping for later implementation

    XEvent event;
    XSelectInput(dpy, win, KeyPressMask | StructureNotifyMask);

    gap_hl = WIDTH/192;

    while (1) {
        XNextEvent(dpy, &event);
        if(event.type == KeyPress) {

            KeySym keysym = XLookupKeysym(&event.xkey, 0);
//            printf("%lu\n", keysym);
            switch (keysym) {
                case XK_Right:
                    XSetForeground(dpy, gc, BACKGROUND);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);
                    selected = (selected + 1) % BOXES;
                    XSetForeground(dpy, gc, HIGHLIGHT);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);

                    break;
                case XK_Left:
                    XSetForeground(dpy, gc, BACKGROUND);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);
                    selected = ((selected - 1) + BOXES) % BOXES;
                    XSetForeground(dpy, gc, HIGHLIGHT);
                    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);
                    break;
            }
            printf("selected: %d\n", selected);

        }
        else if (event.type == ConfigureNotify) {
            XClearWindow(dpy, win);
            drawSelections(dpy, win, gc, event.xconfigure.width, event.xconfigure.height);
            gap_hl = WIDTH/192;
            XSetForeground(dpy, gc, HIGHLIGHT);
            highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);
        }
    }
}

void scaleMenu(Display * dpy, Window win, GC gc, XEvent event) {
    XClearWindow(dpy, win);
    drawSelections(dpy, win, gc, event.xconfigure.width, event.xconfigure.height);
    gap_hl = WIDTH/192;
    XSetForeground(dpy, gc, HIGHLIGHT);
    highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], gap_hl);
    XFlush(dpy);
}

void menuLoop (Display* dpy, Window win, GC gc, Controller dev, int rc, struct pollfd* events) {
    struct input_event evdev; //Loops through all events in queue
    struct input_event prominentEvent; //Desired event pulled out of queue
    struct input_event pastEvent; //Previous event used for joystick control

    XEvent event;
    int width = WIDTH;
    int height = HEIGHT;
    XSelectInput(dpy, win, StructureNotifyMask);

    int middle = (dev.joyMax + dev.joyMin)/2;

    int threshold = (dev.joyMax - middle) - ((dev.joyMax - middle)/4);

    printf("Middle: %d Threshold: %d\n", middle, threshold);

    while (1) {
        usleep(1000);
        if (poll(events, 2, -1) == -1) {
            printf("Something went wrong\n");
        }
        if (events[0].revents & POLLIN) {
            if (rc == LIBEVDEV_READ_STATUS_SYNC || rc == LIBEVDEV_READ_STATUS_SUCCESS || rc == -EAGAIN) {
                if (libevdev_has_event_pending(dev.device)) {
                    //Reset the event grabbed from queue
                    prominentEvent.type = 0;
                    prominentEvent.value = 0;
                    while (libevdev_has_event_pending(dev.device)) { //Will loop through all events queued
                        rc = libevdev_next_event(dev.device,
                                                 LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, &evdev);
                        if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
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
                                selected = selectBox(dpy, win, gc, prominentEvent.value - middle, pastEvent.value - middle, threshold);
                                //Past event prevents selected from being changed if the joystick was in a similar position when selected was changed
                                pastEvent = prominentEvent;
                            } else if (prominentEvent.code == ABS_HAT0X) {
                                selected = selectBox(dpy, win, gc, prominentEvent.value, pastEvent.value, 1);
                                //printf("selected: %d\n", selected);
                                pastEvent = prominentEvent;
                            }
                            break;
                    }
                }
            }
        }
        if (events[1].revents & POLLIN) {
            XNextEvent(dpy, &event);
            if (event.type == ConfigureNotify && width != event.xconfigure.width && height != event.xconfigure.height) {
//
            }
        }
    }
}
