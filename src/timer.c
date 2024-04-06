#include "timer.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

instant instant_now() {
    instant now;
    getrusage(RUSAGE_SELF, &now.rtime);
    gettimeofday(&now.tv, NULL);
    return now;
}

#define PRECISION ".5"

double tv_elapsed(struct timeval after, struct timeval before) {
    struct timeval diff;
    timersub(&after, &before, &diff);
    return (double)diff.tv_sec + diff.tv_usec/1000000.0;
}

void instant_print_elapsed(instant after, instant before) {
    #ifndef MACHINE
    double sys = tv_elapsed(after.rtime.ru_stime, before.rtime.ru_stime);
    double user = tv_elapsed(after.rtime.ru_utime, before.rtime.ru_utime);
    #endif
    double phys = tv_elapsed(after.tv, before.tv);
    #ifndef MACHINE
    printf("\tSystem time: %"PRECISION"lf segundos\n", sys);
    printf("\tUser time: %"PRECISION"lf segundos\n", user);
    printf("\tTempo físico: %"PRECISION"lf segundos\n", phys);
    #else
    printf("%lf\n", phys);
    #endif
}
