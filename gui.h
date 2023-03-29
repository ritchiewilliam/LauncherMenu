//
// Created by william on 28/03/23.
//
#ifndef MENU_GUI_H
#define MENU_GUI_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdlib.h>

#define POSY 0
#define POSX 0
#define WIDTH 1920
#define HEIGHT 1080
#define BORDER 5
#define GAP_HL 10
#define BOXES 5

extern unsigned long background;
extern unsigned long foreground;

typedef struct Position {
    int x;
    int y;
    int width;
    int height;
    int r;

} Position;

void setPosition(Position * pos, int x, int y, int width, int height, int r);

void XFillRoundedRectangle(Display * dpy, Window win, GC gc, Position pos);
void highlightRoundedRectangle(Display * dpy, Window win, GC gc, Position pos, int g);

Position * drawSelections (Display * dpy, Window win, GC gc);

int selectBox (Display * dpy, Window win, GC gc, int promVal, int pastVal, Position * boxPositions, int selected, int threshold);

#endif //MENU_GUI_H
