#pragma once

#include <sys/time.h>
#include <sys/resource.h>

typedef struct {
    struct rusage rtime;
    struct timeval tv;
} instant;

instant instant_now();

double tv_elapsed(struct timeval after, struct timeval before);

void instant_print_elapsed(instant after, instant before);
