#pragma once
#include <stdbool.h>

#include "list.h"

typedef unsigned int vertex;
typedef unsigned int weight;

#define NO_VERTEX ((vertex)~0)

typedef struct {
    vertex v;
    vertex u;
    weight weight;
} edge;

typedef struct vertex_data vertex_data;
typedef struct graph graph;

graph* graph_new();

vertex graph_add_vertex(graph* g);
vertex_data* graph_get_vertex_data(graph* g, vertex v);
list* graph_get_vertices(graph* g);

bool graph_add_edge(graph* g, vertex v, vertex u, weight w);
bool graph_remove_edge(graph* g, vertex v, vertex u);

list* graph_shortest_paths(graph* g, int k, vertex source, vertex dest);

edge* vd_get_edge(vertex_data* vdata, vertex u); 
list* vd_get_edges(vertex_data* vdata); 
vertex vd_get_id(vertex_data* vdata); 

void graph_free(graph* g);
