//
// Created by william on 28/03/23.
//
#ifndef MENU_GUI_H
#define MENU_GUI_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <string.h>

//#include "res/settings.xpm"

#define WIDTH 900
#define HEIGHT 500
#define BORDER 5
//#define GAP_HL 10
#define BOXES 5
#define BACKGROUND 0x2E3440
#define FOREGROUND 0x4C566A
#define FONT
#define HIGHLIGHT 0x8c97ab

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

XFontStruct * scaleFont(Display * dpy, char * font, int size);

void settingsImage(XImage * image);

int centerText(Display * dpy, Window win, GC gc, XFontStruct * font, char * text, int y, int width);


//Position * drawSelections (Display * dpy, Window win, GC gc);

//int selectBox (Display * dpy, Window win, GC gc, int promVal, int pastVal, Position * boxPositions, int selected, int threshold);

#endif //MENU_GUI_H
