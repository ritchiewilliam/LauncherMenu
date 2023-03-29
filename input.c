#include <stdlib.h>
#include "input.h"

controller* init_input(const char * file) {

    controller* con = (controller*)malloc(sizeof(controller));

    int fd;
    int rc = 1;

    struct libevdev *dev = NULL;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }

    rc = libevdev_new_from_fd(fd, &dev);

    if(rc < 0) {
        libevdev_free(dev);
        return NULL;
    }
    con->device = dev;
    con->rc = rc;
    return con;
}