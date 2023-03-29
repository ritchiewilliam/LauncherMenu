#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

static Display* dpy;
static int scr;
static Window win;
static GC gc;

#define POSY 0
#define POSX 0
#define WIDTH 1920
#define HEIGHT 1080
#define BORDER 5

#define GAP_HL 10
#define BOXES 5

unsigned long background = 0x2E3440;
unsigned long foreground = 0x4C566A;

typedef struct Position {
    int x;
    int y;
    int width;
    int height;
    int r;

} Position;

unsigned long RGB(int r, int g, int b) {
    return b + (g<<8) + (r<<16);
}

void random_RGB(int * rgb)
{
    int i;
    for(i=0;i<3;i++)
    {
        rgb[i]=rand()%256;
    }
}

int rando (int bounds) {
    return rand() % (bounds + 1);
}

void setPosition(Position * pos, int x, int y, int width, int height, int r) {
    pos->x = x;
    pos->y = y;
    pos->width = width;
    pos->height = height;
    pos->r = r;
}
//void XFillRoundedRectangle(int x, int y, int width, int height, int r) {
//
//    int c = 2 * r;
//
//    int xRight = x + width - c;
//    int yBottom = y + height - c;
//
//    XFillArc(dpy, win, gc, xRight, y, c, c, 90 * 64, -(90 * 64));
//    XFillArc(dpy, win, gc, x, y, c, c, 90 * 64, 90 * 64);
//    XFillArc(dpy, win, gc, x, yBottom, c, c, 180 * 64, 90*64);
//    XFillArc(dpy, win, gc, xRight, yBottom, c, c, 270 * 64, 90*64);
//    XFillRectangle(dpy, win, gc, x + r, y, width - c, height);
//    XFillRectangle(dpy, win, gc, x, y + r, width, height - c);
//
//}

//void highlightRoundedRectangle(int x, int y, int width, int height, int r, int g) {
//
//    r += g;
//    int c = 2 * r;
//
//    int xRightC = x + width - c + g;
//    int yBottomC = y + height - c + g;
//
//    int xRightL = x + width - r + g;
//    int yBottomL = y + height - r + g;
//
//    int xLeftL = x + r - g;
//    int yTopL = y + r - g;
//    XDrawArc(dpy, win, gc, xRightC, y - g, c, c, 90 * 64, -(90 * 64));
//    XDrawArc(dpy, win, gc, x - g, y - g, c, c, 90 * 64, 90 * 64);
//    XDrawArc(dpy, win, gc, x - g, yBottomC, c, c, 180 * 64, 90*64);
//    XDrawArc(dpy, win, gc, xRightC, yBottomC, c, c, 270 * 64, 90*64);
//    XDrawLine(dpy, win, gc, x - g, yTopL, x - g, yBottomL);
//    XDrawLine(dpy, win, gc, xRightL + r, yTopL, xRightL + r, yBottomL);
//    XDrawLine(dpy, win, gc, xLeftL, y - g, xRightL, y - g);
//    XDrawLine(dpy, win , gc, xRightL, yBottomL + r, xLeftL, yBottomL + r);
//}

void XFillRoundedRectangle(Position pos) {
    int c = 2 * pos.r;

    int xRight = pos.x + pos.width - c;
    int yBottom = pos.y + pos.height - c;

    XFillArc(dpy, win, gc, xRight, pos.y, c, c, 90 * 64, -(90 * 64));
    XFillArc(dpy, win, gc, pos.x, pos.y, c, c, 90 * 64, 90 * 64);
    XFillArc(dpy, win, gc, pos.x, yBottom, c, c, 180 * 64, 90*64);
    XFillArc(dpy, win, gc, xRight, yBottom, c, c, 270 * 64, 90*64);
    XFillRectangle(dpy, win, gc, pos.x + pos.r, pos.y, pos.width - c, pos.height);
    XFillRectangle(dpy, win, gc, pos.x, pos.y + pos.r, pos.width, pos.height - c);
}

void highlightRoundedRectangle(Position pos, int g) {

    pos.r += g;
    int c = 2 * pos.r;

    int xRightC = pos.x + pos.width - c + g;
    int yBottomC = pos.y + pos.height - c + g;

    int xRightL = pos.x + pos.width - pos.r + g;
    int yBottomL = pos.y + pos.height - pos.r + g;

    int xLeftL = pos.x + pos.r - g;
    int yTopL = pos.y + pos.r - g;
    XDrawArc(dpy, win, gc, xRightC, pos.y - g, c, c, 90 * 64, -(90 * 64));
    XDrawArc(dpy, win, gc, pos.x - g, pos.y - g, c, c, 90 * 64, 90 * 64);
    XDrawArc(dpy, win, gc, pos.x - g, yBottomC, c, c, 180 * 64, 90*64);
    XDrawArc(dpy, win, gc, xRightC, yBottomC, c, c, 270 * 64, 90*64);
    XDrawLine(dpy, win, gc, pos.x - g, yTopL, pos.x - g, yBottomL);
    XDrawLine(dpy, win, gc, xRightL + pos.r, yTopL, xRightL + pos.r, yBottomL);
    XDrawLine(dpy, win, gc, xLeftL, pos.y - g, xRightL, pos.y - g);
    XDrawLine(dpy, win , gc, xRightL, yBottomL + pos.r, xLeftL, yBottomL + pos.r);
}

Position * drawSelections (int boxes) {
    int boxWidth = 300;
    int boxHeight = 500;
    int gap = boxWidth + 75;
    int alt = 1;
    int x = (WIDTH / 2) - (boxWidth / 2);

    XSetForeground(dpy, gc, foreground);

    int index = boxes / 2;

    Position * boxPositions = (Position *)malloc(sizeof(Position) * boxes);

    for(int i = 0; i < boxes; i++) {
        alt = -alt;
        x += gap * alt * i;

        index += i * alt;

        setPosition(&boxPositions[index], x, 200, boxWidth, boxHeight, 20);

        //XFillRectangle(dpy, win, gc, x, 200, boxWidth, boxHeight);
        XFillRoundedRectangle(boxPositions[index]);
        printf("Location: %d to %d\n\n", x, x + boxWidth);
    }
    XFlush(dpy);

    return boxPositions;
}

int selectBox (int promVal, int pastVal, Position * boxPositions, int selected, int threshold) {

    int increment = ((promVal >= threshold && pastVal <= threshold) - (promVal <= -threshold && pastVal >= -threshold));

    if(abs(increment)) {
        XSetForeground(dpy, gc, background);
        highlightRoundedRectangle(boxPositions[selected], GAP_HL);

        selected = (selected + increment + 5) % 5;

        XSetForeground(dpy, gc, 0x8c97ab);
        highlightRoundedRectangle(boxPositions[selected], GAP_HL);

        XFlush(dpy);
    }

//    printf("%d, %d, %d\n", evdev->code == 0 && evdev->value > 16000, evdev->code == 0 && evdev->value < -16000, selected);
    return selected;
}

int main() {

    const char* file = "/dev/input/event24";
    struct controller* dev = init_input(file);

    dpy = XOpenDisplay(NULL);
    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, WIDTH, HEIGHT, 0, foreground, background);

    XGCValues mask;

    mask.line_width = 5;

    XEvent event;

    XSelectInput(dpy, win, KeyPressMask);

    gc = XCreateGC(dpy, win, GCLineWidth, &mask);

    XMapWindow(dpy, win);
    XFlush(dpy);
    usleep(20000);

    Position * boxPositions;

    if(dev->device == NULL) {
        printf("No controller connected\n");
        return 0;

//        struct input_event evdev;
    }
    boxPositions = drawSelections(BOXES);

    int selected = 0;

    highlightRoundedRectangle(boxPositions[0], GAP_HL);

    int * rgb = (int*)malloc(sizeof(int) * 3);
    unsigned long color = 0x4C566A;

    int rc = 1;



    clock_t start, end;

    start = clock();

    struct input_event evdev;
    struct input_event prominentEvent;
    struct input_event pastEvent;



        while (1) {
            //XDrawRectangle(dpy, win, gc, rando(WIDTH), rando(HEIGHT), rando(WIDTH), rando(HEIGHT));
            if (dev->rc == LIBEVDEV_READ_STATUS_SYNC || dev->rc == LIBEVDEV_READ_STATUS_SUCCESS || dev->rc == -EAGAIN) {
////                printf("Hello\n");
//
//                dev->rc = libevdev_next_event(dev->device, LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, &evdev);
//                if (dev->rc == LIBEVDEV_READ_STATUS_SUCCESS) {
//                    do {
//                        end = clock();
////                        printf("%d\n", libevdev_has_event_pending(dev->device));
//                        printf("%d\n", evdev.type);
//                        //            printf("Success!\n");
//                        switch (evdev.type) {
//
//                            case 1:
//                                printf("oi\n");
//                                random_RGB(rgb);
//                                break;
//                            case 3:
//                                if ((end - start) > 250) {
//                                    printf("3oi\n");
//                                    //                                selected = selectBox(&evdev, selected);
//                                    selected = (selected + ((evdev.value > 16000) - (evdev.value < -16000)) * !evdev.code) + 5 % 5;
////                                    printf("%d\n", selected);
//                                    //                                printf("%d\n", evdev.code);
//                                }
//                                pastEvent = evdev;
//                                break;
//
//                            default:
//
//                        }
//                    } while(!libevdev_has_event_pending(dev->device));
//
//                }

                if (libevdev_has_event_pending(dev->device)) {
                    prominentEvent.type = 0;
                    prominentEvent.value = 0;
                    while(libevdev_has_event_pending(dev->device)) {
                        dev->rc = libevdev_next_event(dev->device,
                                                      LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, &evdev);
                        if (dev->rc == LIBEVDEV_READ_STATUS_SUCCESS) {
                            if(evdev.type == 1) {
                                prominentEvent = evdev;
                                prominentEvent.type = !(prominentEvent.value);
                            }
                            else if(evdev.type == 3) {
                                if(abs(evdev.value) > abs(prominentEvent.value)) {
                                    prominentEvent = evdev;
                                }
                            }
                        }
                    }
                        switch(prominentEvent.type) {
                        case 1:
                                printf("Code: %d Value: %d\n", pastEvent.code, pastEvent.value);
                            break;
                            case 3:
                                if(!prominentEvent.code) {
                                    selected = selectBox(prominentEvent.value, pastEvent.value, boxPositions, selected, 22000);
                                    printf("selected: %d\n", selected);
                                    pastEvent = prominentEvent;
                                }
                                else if (prominentEvent.code == 16) {
                                    selected = selectBox(prominentEvent.value, pastEvent.value, boxPositions, selected, 1);
                                    printf("selected: %d\n", selected);
                                    pastEvent = prominentEvent;
                                }
                            break;
                    }
                }
            }
        usleep(10000);
//        }
    }

    XUnmapWindow(dpy,win);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}

//void keyboardLoop() {
//    while (1) {
//        XNextEvent(dpy, &event);
//        if(event.type == KeyPress) {
//
//            KeySym keysym = XLookupKeysym(&event.xkey, 0);
////            printf("%lu\n", keysym);
//            switch (keysym) {
//                case XK_Right:
//                    XSetForeground(dpy, gc, background);
//                    highlightRoundedRectangle(boxPositions[selected], 5);
//                    selected = (selected + 1) % 5;
//                    XSetForeground(dpy, gc, 0x8c97ab);
//                    highlightRoundedRectangle(boxPositions[selected], 5);
//
//                    break;
//                case XK_Left:
//                    XSetForeground(dpy, gc, background);
//                    highlightRoundedRectangle(boxPositions[selected], 5);
//                    selected = ((selected - 1) + 5) % 5;
//                    XSetForeground(dpy, gc, 0x8c97ab);
//                    highlightRoundedRectangle(boxPositions[selected], 5);
//                    break;
//            }
//            printf("selected: %d\n", selected);
//
//        }
//    }
//}