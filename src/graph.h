#pragma once
#include <stdbool.h>

typedef struct graph graph;
typedef unsigned short int vertex;
typedef unsigned short int weight;

graph* graph_new();

vertex graph_add_vertex(graph* g);
bool graph_add_edge(graph* g, vertex v, vertex u, weight w);

void graph_free(graph* g);
