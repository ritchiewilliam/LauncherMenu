#include "menu.h"
#include "gui.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <X11/xpm.h>

Display* dpy;
//static int scr;
Window win;
GC gc;

//int WIDTH = 800;
//int HEIGHT = 500;
int ctlFd;

static void quit() {
    XFreeGC(dpy, gc);
    XUnmapWindow(dpy,win);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    if(ctlFd != -1) {
        close(ctlFd);
    }
    exit(0);
}

void init() {
    dpy = XOpenDisplay(NULL);
//
//    WIDTH = DisplayWidth(dpy, 0);
//    HEIGHT = DisplayHeight(dpy, 0);

    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, WIDTH, HEIGHT, 0, FOREGROUND, BACKGROUND);

    XGCValues mask;
    mask.line_width = WIDTH / 384;
    gc = XCreateGC(dpy, win, GCLineWidth, &mask);

    XMapWindow(dpy, win);
    XFlush(dpy);

    usleep(20000);
}

//void scale() {
//    gap_hl = WIDTH/192;
//    int r = WIDTH / 96;
//
//
//}

int main() {

    atexit(quit);

    init();
    drawSelections(dpy, win, gc, WIDTH, HEIGHT);
    XFlush(dpy);
//    XImage *image;
//    XpmReadFileToImage(dpy, "res/settings.xpm", &image, NULL, NULL);
//    XPutImage(dpy, win, gc, image, 0, 0, 20, 20, image->width, image->height);

//    XSetForeground(dpy, gc, 0x8c97ab);
//    highlightRoundedRectangle(dpy, win, gc, boxPositions[0], gap_hl);
//    XFlush(dpy);

    char ** controllers = configure_input();
    struct Controller* dev = (Controller*)malloc(sizeof(Controller));



    if(controllers == NULL) {
        printf("No controller connected, using keyboard\n");
        keyboardLoop(dpy, win, gc);
    }
    else {
        printf("%s\n", controllers[0]);
        init_input(dev, controllers[0]);
        ctlFd = open(controllers[0], O_RDONLY);
        controllerLoop(dpy, win, gc, dev, ctlFd);
    }

    quit();
}


