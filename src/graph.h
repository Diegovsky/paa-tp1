#pragma once
#include <stdbool.h>

#include "list.h"

typedef unsigned int vertex;
typedef unsigned int weight;

#define NO_VERTEX ((vertex)~0)

typedef struct {
    vertex v;
    vertex u;
    weight ghost;
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

list* graph_dijkstra(graph* g, vertex source, vertex dest);

edge* vertex_data_get_edge(vertex_data* vdata, vertex u); 
list* vertex_data_get_edges(vertex_data* vdata); 
vertex vertex_data_get_id(vertex_data* vdata); 

void graph_free(graph* g);
