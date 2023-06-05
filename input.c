#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "input.h"

char * dir = "/dev/input/";
//void freeController(Controller * con) {
//    libevdev_free(con->device);
//    free(con);
//}

int initInput(Controller * con, const char * file, int * fd) {

    int rc = 1;

    struct libevdev *dev = NULL;

    int fileDesc = open(file, O_RDONLY);
    if (fileDesc < 0) {
        return -1;
    }

    rc = libevdev_new_from_fd(fileDesc, &dev);

    if(rc < 0) {
        libevdev_free(dev);
        return rc;
    }
    *fd = fileDesc;
    con->device = dev;

    con->joyMax = libevdev_get_abs_maximum(con->device, ABS_X);
    con->joyMin = libevdev_get_abs_minimum(con->device, ABS_X);

    return rc;
}

char ** findInput(int * devices) {

    struct dirent ** names;

    int n = scandir(dir, &names, NULL, alphasort);

    if(n == -1) {
        exit(EIO);
    }

    char * type = "event";
    int e = (int)strlen(type);
    char * checkNames = (char*)malloc(sizeof(char) * (e + 1));
    checkNames[e] = '\0';

    Controller con;

    char ** deviceFiles = (char **)malloc(sizeof(char*) * n);
    int i = 0;
    int j = n;
    int fd;
    while(j--) {
        memcpy(checkNames, names[j]->d_name, e);
        //printf("%d, %s, %s\n",j, type, checkNames);

        //deviceFiles[n - j] = NULL;
        if(!strcmp(type, checkNames)) {
            deviceFiles[i] = (char*)calloc(sizeof(char), strlen(dir) + strlen(names[j]->d_name));
            strcat(deviceFiles[i], dir);
            strcat(deviceFiles[i], names[j]->d_name);
//            printf("%s\n", deviceFiles[i]);
//            libevdev_change_fd(con->device, open(deviceFiles[i], O_RDONLY));
            if(initInput(&con, deviceFiles[i], &fd) >= 0) {
                if (libevdev_has_event_type(con.device, 3)) {
                    i++;
//                    printf("%s\n", libevdev_get_name(con->device));
                }
                else {
                    free(deviceFiles[i]);
                }
                libevdev_free(con.device);
                close(fd);
            }
            else {
                free(deviceFiles[i]);
            }
        }
    }
    *devices = i;
    deviceFiles = realloc(deviceFiles, sizeof(char*) * i);
    free(checkNames);
//    printf("%d\n", i);
    if(i != 0) {
        return deviceFiles;
    }
    free(deviceFiles);
    return NULL;
}

void controlLoop(void (*f)(Display*, Window, GC, Controller, int, struct pollfd*), Display * dpy, Window win, GC gc, Controller dev, int ctlFd, int rc) {
    struct pollfd events[2];

    events[0].fd = ctlFd;
    events[0].events = POLLIN;

    events[1].fd = ConnectionNumber(dpy);
    events[1].events = POLLIN;

    (*f)(dpy, win, gc, dev, rc, events);
}
//int loadBounds(Controller * dev) {
//    FILE * cachedControls = fopen("./cache.txt", "r");
//
//    if (cachedControls != NULL) {
//        char name[100];
//        int bound1;
//        int bound2;
//        dev->joyBounds = 0;
//        while (fscanf(cachedControls, "%99s:%d, %d", name, &bound1, &bound2) != EOF) {
//            if (!strcmp(name, libevdev_get_name(dev->device))) {
//                //int bound = bound1 > bound2 ? bound1 : bound2;
//                dev->joyCenter = abs((bound1 - bound2) / 2);
//                dev->joyBounds = abs(bound1 - dev->joyCenter);
//                return 0;
//            }
//        }
//    }
//    return -1;
//}
char * font = "-misc-open sans-bold-r-normal--0-0-1000-1000-p-0-adobe-standard";

char * selectInput(Display * dpy, Window win, GC gc, char** deviceFiles, int n) {

    char ** deviceNames = (char**)malloc(sizeof(const char*) * (n + 1));
    deviceNames[n] = "Keyboard";
    Controller con;

    for(int i = 0; i < n; i++) {
        int fd;
        if(initInput(&con, deviceFiles[i], &fd) != -1) {
            deviceNames[i] = (char *)malloc(sizeof(char) * (strlen(libevdev_get_name(con.device)) + 1));
            strcpy(deviceNames[i], libevdev_get_name(con.device));
            libevdev_free(con.device);
            close(fd);
        }
    }

    int selected = 0;

    XEvent event;
    XSelectInput(dpy, win, KeyPressMask | StructureNotifyMask);

    XFontStruct * ft = scaleFont(dpy, font, 50);
    XSetFont(dpy, gc, ft->fid);
    centerText(dpy, win, gc, ft, "Select Input", HEIGHT/5, WIDTH);
    XFreeFont(dpy, ft);
    int size = 20;
    ft = scaleFont(dpy, font, size);
    XSetFont(dpy, gc, ft->fid);

//    for (int i = 0; i < n; i++) {
//        printf("%s\n", deviceNames[i]);
//    }
    int width = WIDTH;
    int height = HEIGHT;
    XSetForeground(dpy, gc, HIGHLIGHT);
    int x = centerText(dpy, win, gc, ft, deviceNames[selected], height/2, width);
//    XFree(ft);

    while(1) {
        XNextEvent(dpy, &event);
        if(event.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            if(keysym == XK_Right) {
                selected = (selected + 1) % (n + 1);
                XSetForeground(dpy, gc, BACKGROUND);
                XFillRectangle(dpy, win, gc, x, (height/2)-size, x + ((width/2) - x) * 2, size + (size/4));
                XSetForeground(dpy, gc, HIGHLIGHT);
                x = centerText(dpy, win, gc, ft, deviceNames[selected], height/2, width);
                XFlush(dpy);
            }
            else if (keysym == XK_Left) {
                selected = (selected + n) % (n + 1);
                XSetForeground(dpy, gc, BACKGROUND);
                XFillRectangle(dpy, win, gc, x, (height/2)-size, x + ((width/2) - x) * 2, size + (size / 4));
                XSetForeground(dpy, gc, HIGHLIGHT);
                x = centerText(dpy, win, gc, ft, deviceNames[selected], height/2, width);
                XFlush(dpy);
            }
            else if (keysym == XK_Return) {
                if(selected < n){
                    XClearWindow(dpy, win);
                    char * deviceFile = (char*)malloc(sizeof(char) * strlen(deviceFiles[selected]));
                    strcpy(deviceFile, deviceFiles[selected]);
                    for(int i = 0; i < n; i++) {
                        printf("%s\n", deviceNames[i]);
                        free(deviceNames[i]);
                    }
                    free(deviceNames);
                    return deviceFile;
                }
                return NULL;
            }

        }
        else if (event.type == ConfigureNotify) {

        }
    }
}
