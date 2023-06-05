#include "gui.h"
#include <stdio.h>

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

XFontStruct * scaleFont(Display * dpy, char * font, int size) {
    //"-misc-open sans light-light-r-normal--0-0-1000-1000-p-0-adobe-standard"

    int split = 0;
    //Can scale pixel size using the parameter after 7th dash, find the 7th dash
    for(int i = 0; i < 7; split++) {
        i += (font[split] == '-');
        if(font[i] == 0) {
            return NULL;
        }
    }
    //Inserting new size into 7th parameter slot

    //Split first half of string
    char first[split + 1];
    first[split] = 0;
    memcpy(first, font, split);

    //Triming out previous parameter if any parameter was placed in the string
    int trim;
    for(trim = split; font[trim] != '-'; trim++);
    trim -= split;

    //Length of the string plus \0
    int length = (int)strlen(font) + 1;

    char second[length - split - trim + 1];
    memcpy(second, font + split + trim, length - split - trim);

    size += !size; //Don't want to do log10 of 0

    //The length of the original string, take away original scale, adding the number of digits in the new size
    char new[length-trim+(int)log10(size)+1];
    sprintf(new, "%s%d%s", first, size, second);
//    printf("%s\n", new);

    return XLoadQueryFont(dpy, new);
}

int centerText(Display * dpy, Window win, GC gc, XFontStruct * font, char * text, int y, int width) {

    int x = (width/2) - (XTextWidth(font, text, (int)strlen(text)) / 2);

    XDrawString(dpy, win, gc, x, y, text, (int)strlen(text));

    return x;
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
