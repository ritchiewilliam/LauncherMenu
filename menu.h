#ifndef MENU_MENU_H
#define MENU_MENU_H

#include "gui.h"
#include "input.h"


void drawSelections (Display * dpy, Window win, GC gc, int width, int height);

void keyboardLoop(Display * dpy, Window win, GC gc);

//void controllerLoop(Display * dpy, Window win, GC gc, Controller dev, int ctlFd, int rc);

void controlLoop(void (*f)(Display*, Window, GC, Controller, int, struct pollfd*), Display * dpy, Window win, GC gc, Controller dev, int ctlFd, int rc);


#endif //MENU_MENU_H
