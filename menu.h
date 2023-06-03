#ifndef MENU_MENU_H
#define MENU_MENU_H

#include "gui.h"
#include "input.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

void drawSelections (Display * dpy, Window win, GC gc, int width, int height);

void keyboardLoop(Display * dpy, Window win, GC gc);

void controllerLoop(Display * dpy, Window win, GC gc, Controller * dev, int ctlFd);


#endif //MENU_MENU_H
