#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "graph.h"
#include "list.h"

void graph_to_dot(graph* g) {
    puts("digraph {");
    list_foreach(g->vertices, vertex_data, vdata) {
        list_foreach(vdata->edges, edge, e) {
            printf("\t%c -> %c [label=%.1f]\n", e->v + 'a', e->u + 'a', e->weight);
        }
    }
    puts("}");
}


list* graph_dijkstra(graph* g, vertex source, vertex dest) {
    const size_t vertex_count = g->vertices->len;
    vertex current = source;

    // inicialização do vetor de distancia e caminho
    weight distance[vertex_count];
    vertex prev[vertex_count];
    vertex visited[vertex_count];
    for(int i = 0; i < g->vertices->len; i++) {
        distance[i] = INFINITY;
        prev[i] = NO_VERTEX;
        visited[i] = 0;
    }
    distance[current] = 0;

    for(;;) {
        // Get vertex u with minimal distance from source;
        vertex u = NO_VERTEX;
        for(vertex i = 0; i < vertex_count; i++) {
            if(visited[i]) continue;
            else if(u == NO_VERTEX) u = i;

            if(distance[u] > distance[i]) {
                u = i;
            }
        }
        // if we couldn't find any vertex or we found target, the set is empty.
        if(u == NO_VERTEX || u == dest) goto end;

        // Mark u as visited.
        visited[u] = 1;

        // Find which path is shortest.
        vertex_data* udata = graph_get_vertex_data(g, u);
        list_foreach(udata->edges, edge, e) {
            vertex v = e->u;
            if(visited[v]) continue;
            weight alt = distance[u] + vertex_data_get_edge(udata, v)->weight;
            if (alt < distance[v]) {
                distance[v] = alt;
                prev[v] = u;
            }
        }
    }
    end:;

    list* path = list_new(sizeof(vertex));
    for(vertex u = dest; u != NO_VERTEX; u = prev[u]) {
        list_push(path, &u);
    }

    // reverse path
    vertex* path_start = (vertex*) path->elements;
    vertex* path_end = ((vertex*) path->elements) + path->len-1;
    while(path_start < path_end) {
        vertex temp = *path_start;
        *path_start = *path_end;
        *path_end = temp;

        path_start++;
        path_end--;
    }

    return path;
}

int main() {
    graph* g = graph_new();

    vertex a = graph_add_vertex(g);
    vertex b = graph_add_vertex(g);
    vertex c = graph_add_vertex(g);
    vertex d = graph_add_vertex(g);
    vertex e = graph_add_vertex(g);

    graph_add_edge(g, a, b, 4);
    graph_add_edge(g, a, d, 5);
    graph_add_edge(g, b, d, 2);
    graph_add_edge(g, c, d, 0);
    graph_add_edge(g, e, a, 1);
    graph_add_edge(g, d, e, 9);
    graph_add_edge(g, e, c, 1);

    // graph_to_dot(g);

    list* path = graph_dijkstra(g, a, d);

    bool visited[g->vertices->len];
    memset(visited, false, sizeof(visited));

    list_foreach(path, vertex, v) {
        fprintf(stderr, "%c\n", 'a' + *v);
        visited[*v] = true;
    }

    puts("digraph {");
    list_foreach(g->vertices, vertex_data, vdata) {
        const char node_id = vdata->id + 'a';
        if(visited[vdata->id]) {
            printf("\t%c [color=red]\n", node_id);
        }
        list_foreach(vdata->edges, edge, e) {
            printf("\t%c -> %c [label=%.1f]\n", node_id, e->u + 'a', e->weight);
        }
    }
    puts("}");

    list_free(path);
    graph_free(g);
    return 0;
}
