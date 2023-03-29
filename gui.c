#include "gui.h"
//unsigned long RGB(int r, int g, int b) {
//    return b + (g<<8) + (r<<16);
//}
//
//void random_RGB(int * rgb)
//{
//    int i;
//    for(i=0;i<3;i++)
//    {
//        rgb[i]=rand()%256;
//    }
//}
unsigned long background = 0x2E3440;
unsigned long foreground = 0x4C566A;

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

Position * drawSelections (Display * dpy, Window win, GC gc) {
    int boxWidth = 300;
    int boxHeight = 500;
    int gap = boxWidth + 75;
    int alt = 1;
    int x = (WIDTH / 2) - (boxWidth / 2);

    XSetForeground(dpy, gc, foreground);

    int index = BOXES / 2;

    Position * boxPositions = (Position *)malloc(sizeof(Position) * BOXES);

    for(int i = 0; i < BOXES; i++) {
        alt = -alt;
        x += gap * alt * i;

        index += i * alt;

        setPosition(&boxPositions[index], x, 200, boxWidth, boxHeight, 20);

        //XFillRectangle(dpy, win, gc, x, 200, boxWidth, boxHeight);
        XFillRoundedRectangle(dpy, win, gc, boxPositions[index]);
//        printf("Location: %d to %d\n\n", x, x + boxWidth);
    }
    XFlush(dpy);

    return boxPositions;
}

int selectBox (Display * dpy, Window win, GC gc, int promVal, int pastVal, Position * boxPositions, int selected, int threshold) {

    //If the value pulled from the queue is past the threshold, and the previous value isn't in the same range, change can become 1 or -1
    int change = ((promVal >= threshold && pastVal <= threshold) - (promVal <= -threshold && pastVal >= -threshold));

    if(abs(change)) {
        XSetForeground(dpy, gc, background);
        highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], GAP_HL);

        selected = (selected + change + BOXES) % BOXES;

        XSetForeground(dpy, gc, 0x8c97ab);
        highlightRoundedRectangle(dpy, win, gc, boxPositions[selected], GAP_HL);

        XFlush(dpy);
    }
    return selected;
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