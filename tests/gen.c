#include "../src/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

edge* get_edge(graph* g, vertex v, vertex u) {
    list* edges = graph_list_edges_of(g, v);
    list_foreach(edges, edge, e) {
        if(e->u == u) return e;
    }
    return NULL;
}

int main(int argc, char** argv) {
    if(argc != 4) {
        fprintf(stderr, "Esperava apenas 3 argumentos\n");
        return 1;
    }
    int N, M;
    sscanf(argv[1], "%d", &N);
    sscanf(argv[2], "%d", &M);
    FILE* outfile = fopen(argv[3], "w");
    srand(time(NULL));
    graph* g = graph_new(N);
    for(int i = 0; i < N; i++) {
        graph_add_vertex(g);
    }
    for(int i = 0; i < M; i++) {
        int v = rand() % N;
        int u;
        do { u = rand() % N; } while(u == v);
        int w = rand() % 10000;
        graph_add_edge(g, v, u, w);
    }
    list* nodelist = list_new(sizeof(vertex));
    vertex v = 0;
    list_push(nodelist, &v);
    int visited[N];
    memset(visited, 0, sizeof(visited));
    while(nodelist->len > 0) {
        v = *(vertex*)list_get_unchecked(nodelist, nodelist->len-1);
        list_remove(nodelist, nodelist->len-1);
        if(visited[v]) continue;

        visited[v] = 1;

        list* children = graph_list_edges_of(g, v);
        list_foreach(children, edge, e) {
            list_push(nodelist, &e->u);
        }
    }

    // Caso o último vértice não tenha sido atingido, adicionamos arestas extras
    // para garantir.
    for(int i = N-1; i > 0; i++) {
        if(visited[i]) break;
        M++;
        graph_add_edge(g, i-1, i, rand() % 10000);
    }
    fprintf(outfile, "%d %d %d\n", N, M, 10);
    for(int i = 0; i < N; i ++) {
        list* children = graph_list_edges_of(g, i);
        list_foreach(children, edge, e) {
            fprintf(outfile, "%d %d %ld\n", e->v, e->u, e->weight);
        }
    }

    list_free(nodelist);
    graph_free(g);
    fclose(outfile);
}
