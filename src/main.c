#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "graph.h"
#include "list.h"

struct tp_info {
    graph* g;
    int k;
    int end;
    int start;
};
struct tp_info parse_info(FILE* input_file) {
    graph* g = graph_new();
    int m, n, k;
    bool letters = false;
    char* line = malloc(256);

    fgets(line, 255, input_file);
    if(strstr(line, "LETTER") != NULL) {
        letters = true;
        fgets(line, 255, input_file);
    }
    sscanf(line, "%d %d %d\n", &n, &m, &k);

    list* cities_list = list_new(sizeof(vertex));

    for(int i = 0; i < n; i++) {
        vertex v = graph_add_vertex(g);
        list_push(cities_list, &v);
    }

    int u, v;
    weight w;
    vertex* cities = cities_list->elements;
    for(int i = 0; i < m; i++) {
        do
            if(fgets(line, 255, input_file) == NULL) goto end;
        while(strcspn(line, "\n") == 0);
        if(!letters) {
            sscanf(line, "%d %d %d\n", &v, &u, &w);
            v = cities[v-1];
            u = cities[u-1];
        } else {
            sscanf(line, "%c %c %d\n", (char*)&v, (char*)&u, &w);
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

void graph_to_dot(graph* g, list* path) {
    list* vertices = graph_get_vertices(g);
    bool visited[vertices->len];
    vertex edge_next[vertices->len];
    memset(visited, false, sizeof(visited));
    memset(edge_next, NO_VERTEX, sizeof(edge_next));

    if(path) {
        list_foreach(path, edge*, eptr) {
            edge* e = *eptr;
            visited[e->v] = true;
            visited[e->u] = true;
            edge_next[e->v] = e->u;
        }
    }

    puts("digraph {");
    list_foreach(graph_get_vertices(g), vertex_data, vdata) {
        vertex id = vd_get_id(vdata);
        printf("\t%d [label=%c", id, id+'a');
        if(visited[id]) {
            printf("; color=red");
        }
        printf("];\n");
        list* edges = vd_get_edges(vdata);
        list_foreach(edges, edge, e) {
            printf("\t%d -> %d [label=", id, e->u);
            printf("%d", e->weight);
            if(edge_next[e->v] == e->u) {
                printf("; color=green");
            }
            printf("];\n");
        }
    }
    puts("}");
}

int edge_weight_cmp(const edge* e1, const edge* e2){ 
    return e1->weight - (e2->weight);
}

int edge_cmp(const edge* e1, const edge* e2){ 
    return !(e1->u == e2->u && e1->v == e2->v); 
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
    struct tp_info info = parse_info(input_file);
    graph* g = info.g;
    // graph* g = simple_graph();

    graph_to_dot(g, NULL);

    for(int i = 0; i < info.k; i++) {
        list* path = graph_dijkstra(g, info.start, info.end);
        graph_to_dot(g, path);

        // edge* e = list_min(path, (element_comparator_fn)edge_weight_ghost_cmp);

        /* if(last_path && list_eq(last_path, path, (element_comparator_fn)edge_cmp)) {
            i--;
            continue;
        } */

        int total = 0;
        list_foreach(path, edge*, e) {
            total += (*e)->weight;
        }
        fprintf(output_file, "%d ", total);
        /* if(!last_path)
            last_path = path;
        else
            list_free(path); */
    }
    fputs("\n", output_file);

    graph_free(g);
    return 0;
}
