#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"

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

edge* vertex_data_get_edge(vertex_data* vdata, vertex u) {
    list_foreach(vdata->edges, edge, e) {
        if(e->u == u) {
            return e;
        }
    }
    return NULL;
}

void graph_free(graph* g) {
    list_foreach(g->vertices, vertex_data, vdata) {
        list_free(vdata->edges);
    }
    list_free(g->vertices);
    free(g);
}

