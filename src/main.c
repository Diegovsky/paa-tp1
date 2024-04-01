#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/time.h>
#include <sys/resource.h>

#include "graph.h"
#include "heap.h"
#include "list.h"

struct tp_info {
    graph* g;
    int k;
    int end;
    int start;
};
struct tp_info parse_info(FILE* input_file) {
    int m, n, k;
    bool letters = false;
    char* line = malloc(256);

    fgets(line, 255, input_file);
    if(strstr(line, "LETTER") != NULL) {
        letters = true;
        fgets(line, 255, input_file);
    }
    sscanf(line, "%d %d %d\n", &n, &m, &k);
    graph* g = graph_new(n);

    list* cities_list = list_new(sizeof(vertex));

    for(int i = 0; i < n; i++) {
        vertex v = graph_add_vertex(g);
        list_push(cities_list, &v);
    }

    vertex* cities = cities_list->elements;
    for(int i = 0; i < m; i++) {
        int u = 0, v = 0;
        weight w = 0;
        do
            if(fgets(line, 255, input_file) == NULL) goto end;
        while(strcspn(line, "\n") == 0);
        if(!letters) {
            sscanf(line, "%d %d %lu\n", &v, &u, &w);
            v = cities[v-1];
            u = cities[u-1];
        } else {
            sscanf(line, "%c %c %lu\n", (char*)&v, (char*)&u, &w);
            v -= 'a';
            u -= 'a';
        }
        graph_add_edge(g, v, u, w);
    }
    end:;

    int start = cities[0];
    int end = cities[n-1];

    free(line);
    list_free(cities_list);
    return (struct tp_info){
        .g=g,
        .k=k,
        .start=start,
        .end = end,
    };
}

typedef struct {
    struct rusage rtime;
    struct timeval tv;
} instant;

instant instant_now() {
    instant now;
    getrusage(RUSAGE_SELF, &now.rtime);
    gettimeofday(&now.tv, NULL);
    return now;
}

static inline double tv_elapsed(struct timeval after, struct timeval before) {
    struct timeval diff;
    timersub(&after, &before, &diff);
    return (double)diff.tv_sec + diff.tv_usec/1000000.0;
}

static void instant_print_elapsed(instant after, instant before) {
    double sys = tv_elapsed(after.rtime.ru_stime, before.rtime.ru_stime);
    double user = tv_elapsed(after.rtime.ru_utime, before.rtime.ru_utime);
    double phys = tv_elapsed(after.tv, before.tv);
#ifndef MACHINE
    printf("\tSystem time: %.3lf segundos\n", sys);
    printf("\tUser time: %.3lf segundos\n", user);
    printf("\tTempo físico %.3lf segundos\n", phys);
#else
    printf("%.3lf\n", sys);
    printf("%.3lf\n", user);
    printf("%.3lf\n", phys);
#endif
}

int main(int argc, char** argv) {
    FILE* input_file = NULL;
    FILE* output_file = stderr;
    char c;
    while((c = getopt(argc, argv, "i:o:")) != -1) {
        switch (c) {
            case 'i':
                input_file = fopen(optarg, "r");
                if(!input_file) printf("Falha ao abrir arquivo %s\n", optarg);
                break;
            case 'o':
                output_file = fopen(optarg, "w");
                if(!output_file) printf("Falha ao abrir arquivo %s\n", optarg);
                break;
            default:
                ;;
        }
    }
    if(!(input_file && output_file)) {
        if (!input_file) 
            fputs("Faltando arquivo de entrada\n", stderr);
        if (!output_file) 
            fputs("Faltando arquivo de saida\n", stderr);
        return 1;
    }

    instant before = instant_now();
    struct tp_info info = parse_info(input_file);
    instant after = instant_now();
    #ifndef MACHINE
    printf("Tempo de IO:\n");
    #endif
    instant_print_elapsed(after, before);

    graph* g = info.g;

    before = instant_now();
    list* paths = graph_shortest_paths(g, info.k, info.start, info.end);
    after = instant_now();
    #ifndef MACHINE
    printf("Tempo de resolução:\n");
    #endif
    instant_print_elapsed(after, before);

    list_foreach(paths, weight, w) {
        fprintf(output_file, "%lu ", *w);
    }
    fputs("\n", output_file);

    list_free(paths);
    graph_free(g);
    fclose(input_file);
    fclose(output_file);
    return 0;
}
