#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/stat.h>
#include "image.h"

void daemonize() {
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
        close(x);
    }

    openlog("image_server", LOG_PID, LOG_DAEMON);
}

image_data *convert_image_1_svc(image_data *argp, struct svc_req *rqstp) {
    static image_data result;

    result.image_name = strdup(argp->image_name);

    int len = strlen(argp->image_data);
    result.image_data = malloc(len * 2 + 1);

    for (int i = 0; i < len; i++) {
        result.image_data[i * 2] = argp->image_data[i];
        result.image_data[i * 2 + 1] = argp->image_data[i];
    }

    result.image_data[len * 2] = '\0';

    return &result;
}

int main() {
    daemonize();
    openlog("image_server", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Image server started as daemon");
    svc_run();
    return 0;
}

