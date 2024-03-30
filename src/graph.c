#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"


struct graph {
    list* vertices;
};

struct vertex_data {
    list* edges;
    vertex id;
};


static vertex_data vdata_new(vertex id) {
    vertex_data vdata = {.edges = list_new(sizeof(edge)), .id=id};
    return vdata;
}

graph* graph_new() {
    graph* g = malloc(sizeof(graph));
    g->vertices = list_new(sizeof(vertex_data));
    return g;
}

vertex graph_add_vertex(graph* g) {
    vertex v = g->vertices->len;
    vertex_data vdata = vdata_new(v);
    list_push(g->vertices, &vdata);
    return v; 
}

vertex_data* graph_get_vertex_data(graph* g, vertex v) {
    return list_get(g->vertices, v);
}

list* graph_get_vertices(graph* g) {
    return g->vertices;
}

bool graph_add_edge(graph* g, vertex v, vertex u, weight w) {
    vertex_data* vdata = graph_get_vertex_data(g, v);
    vertex_data* udata = graph_get_vertex_data(g, u);
    if (!vdata || !udata) {
        return false;
    }
    edge e = (edge){.v = v, .u = u, .weight = w};
    list_push(vdata->edges, &e);
    return true;
}

bool graph_remove_edge(graph* g, vertex v, vertex u) {
    vertex_data* vdata = graph_get_vertex_data(g, v);
    if (!vdata) {
        return false;
    }
    int i = 0;
    list_foreach(vdata->edges, edge, e) {
        if(e->u == u) {
            return list_remove(vdata->edges, i);
        }
        i++;
    }
    return false;
}

list* graph_dijkstra(graph* g, vertex source, vertex dest) {
    const size_t vertex_count = g->vertices->len;
    vertex current = source;

    // inicialização do vetor de distancia e caminho
    weight distance[vertex_count];
    vertex prev[vertex_count];
    vertex visited[vertex_count];
    for(int i = 0; i < vertex_count; i++) {
        distance[i] = ~0;
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
            weight alt = distance[u] + e->weight;
            if (alt < distance[v]) {
                distance[v] = alt;
                prev[v] = u;
            }
        }
    }
    end:;

    list* path = list_new(sizeof(edge*));
    vertex last = NO_VERTEX;
    for(vertex v = dest; v != NO_VERTEX; v = prev[v]) {
        if(last != NO_VERTEX) {
            vertex_data* vdata = graph_get_vertex_data(g, v);
            edge* e = vd_get_edge(vdata, last);
            list_insert(path, &e, 0);
        }
        last = v;
    }

    return path;
}

edge* vd_get_edge(vertex_data* vdata, vertex u) {
    list_foreach(vdata->edges, edge, e) {
        if(e->u == u) {
            return e;
        }
    }
    return NULL;
}

list* vd_get_edges(vertex_data* vdata) {
    return vdata->edges;
}

vertex vd_get_id(vertex_data* vdata) {
    return vdata->id;
}



void graph_free(graph* g) {
    list_foreach(g->vertices, vertex_data, vdata) {
        list_free(vdata->edges);
    }
    list_free(g->vertices);
    free(g);
}

