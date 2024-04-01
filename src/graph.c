#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"
#include "heap.h"


struct graph {
    list* vertices;
};

struct vertex_data {
    list* edges;
    vertex id;
};

struct cslot {
    weight w;
    char c;
};


static vertex_data vdata_new(vertex id) {
    vertex_data vdata = {.edges = list_new(sizeof(edge)), .id=id};
    return vdata;
}

graph* graph_new(size_t vertex_capacity) {
    graph* g = malloc(sizeof(graph));
    g->vertices = list_new_with_capacity(sizeof(vertex_data), vertex_capacity);
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

list* graph_shortest_paths(graph* g, int k, vertex source, vertex dest) {
    const size_t vertex_count = g->vertices->len;

    int visit_count[vertex_count];
    for(int i = 0; i < vertex_count; i++) {
        visit_count[i] = 0;
    }

    heap* queue = heap_new(sizeof(vertex));
    heap_push(queue, &source, 0);


    list* paths = list_new(sizeof(list*));
    while(visit_count[dest] < k) {
        // Get vertex u with minimal distance from source;
        vertex u;
        weight w;
        // heap vazia;
        if(!heap_pop(queue, &u, &w)) break;
        if(visit_count[u] == k) continue;
        // Mark u as visited.
        visit_count[u] += 1;
        if(u == dest) {
            list_push(paths, &w);
        }

        // Find which path is shortest.
        vertex_data* udata = graph_get_vertex_data(g, u);
        list_foreach(udata->edges, edge, e) {
            vertex v = e->u;
            heap_push(queue, &v, e->weight + w);
        }
    }

    heap_free(queue);

    return paths;
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

