#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include "input.h"

char * dir = "/dev/input/";

void freeController(Controller * con) {
    libevdev_free(con->device);
    free(con);
}

int init_input(Controller * con, const char * file) {

    int fd;
    int rc = 1;

    struct libevdev *dev = NULL;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    rc = libevdev_new_from_fd(fd, &dev);

    if(rc < 0) {
        libevdev_free(dev);
        return 0;
    }
    con->device = dev;
    con->rc = rc;
    return 1;
}

char ** configure_input() {

    struct dirent ** names;

    int n = scandir(dir, &names, NULL, alphasort);

    if(n == -1) {
        exit(EIO);
    }

    char * type = "event";
    int e = (int)strlen(type);
    char * checkNames = (char*)malloc(sizeof(char) * (e + 1));
    checkNames[e] = '\0';

    Controller* con = (Controller*)malloc(sizeof(Controller));

    char ** controlFiles = (char **)malloc(sizeof(char*) * n);
    int i = 0;
    int j = n;
    while(j--) {
        memcpy(checkNames, names[j]->d_name, e);
        //printf("%d, %s, %s\n",j, type, checkNames);

        //controlFiles[n - j] = NULL;
        if(!strcmp(type, checkNames)) {
            controlFiles[i] = (char*)calloc(sizeof(char), strlen(dir) + strlen(names[j]->d_name));
            strcat(controlFiles[i], dir);
            strcat(controlFiles[i], names[j]->d_name);
            //printf("%s\n", controlFiles[i]);
//            libevdev_change_fd(con->device, open(controlFiles[i], O_RDONLY));
            con->rc = init_input(con, controlFiles[i]);
            if(con->rc) {
                if (libevdev_has_event_type(con->device, 3)) {
                    i++;
//                    printf("%s\n", libevdev_get_name(con->device));
                    libevdev_free(con->device);
                } else {
                    libevdev_free(con->device);
                    free(controlFiles[i]);
                }
            }
            else {
                free(controlFiles[i]);
            }
        }
    }
    controlFiles = realloc(controlFiles, sizeof(char*) * i);
    free(checkNames);

    printf("%d\n", i);
    if(i != 0) {
        return controlFiles;
    }
    free(controlFiles);
    return NULL;
}
