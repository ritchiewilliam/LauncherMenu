#include "gui.h"
//unsigned long background = ;
//unsigned long foreground = 0x4C566A;

void XFillRoundedRectangle(Display * dpy, Window win, GC gc, Position pos) {
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

void highlightRoundedRectangle(Display * dpy, Window win, GC gc, Position pos, int g) {

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

void setPosition(Position * pos, int x, int y, int width, int height, int r) {
    pos->x = x;
    pos->y = y;
    pos->width = width;
    pos->height = height;
    pos->r = r;
}

//int stringToInt(char * s, int l) {
//    int value = 0;
//
//    for(int i = l - 1; i >= 0; i++) {
//        value += (int)powf((float)10, (float)(i)) * (s[i] - '0');
//    }
//    return value;
//}

//void settingsImage(Pixmap * image) {
//
//    XCreatePixmapFromBitmapData;
//    XCreatePixmap(dpy, win, 30, 30, 0);
//    image = (XImage *)malloc(sizeof(XImage));
//    int * params = (int *) malloc(sizeof(int)* 4);
//    int start = 0;
//    for(int i, j = 0; settings[1][i]; i++) {
//        if(settings[1][i] == ' ') {
//            int difference = i - start;
//            char * parsed = (char*)malloc(sizeof(char)* i - start);
//            memcpy(parsed, settings[1] + start, i - start + 1);
//            parsed[i - start] = 0;
//            params[j] = stringToInt(parsed, i - start);
//            free(parsed);
//            start = i + 1;
//            j++;
//        }
//    }
//
//    image->width = params[0];
//    image->height = params[1];
//    image->
//
//    for(int i = 0; settings[i]; i++) {
//
//    }
//}

//void loadImage(XImage * image, char * directory) {
//    FILE * fp = fopen(directory, "r");
//    char * line;
//    while() {
//        fgets(line, fp);
//
//    }
//
//
//}

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
