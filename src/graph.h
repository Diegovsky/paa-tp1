#pragma once
#include <stdbool.h>

#include "list.h"

typedef unsigned short int vertex;
typedef float weight;

const vertex NO_VERTEX = ~0;

typedef struct {
    vertex v;
    vertex u;
    weight weight;
} edge;

typedef struct {
    list* edges;
    vertex id;
} vertex_data;

typedef struct {
    list* vertices;
} graph;

graph* graph_new();


vertex graph_add_vertex(graph* g);
vertex_data* graph_get_vertex_data(graph* g, vertex v);
list* graph_get_neighbours(graph*g, vertex v);
bool graph_add_edge(graph* g, vertex v, vertex u, weight w);

edge* vertex_data_get_edge(vertex_data* vdata, vertex u); 

void graph_free(graph* g);
