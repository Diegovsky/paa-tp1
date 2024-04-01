#pragma once
#include <stdbool.h>

#include "list.h"

typedef unsigned int vertex;
typedef unsigned long int weight;

#define NO_VERTEX ((vertex)~0)

/* Representa uma aresta (arco) no grafo.
 * v: nó de origem
 * u: nó de destino
 * w: peso associado.
 */
typedef struct {
    vertex v;
    vertex u;
    weight weight;
} edge;

typedef struct vertex_data vertex_data;
typedef struct graph graph;

/* Cria e inicializa um grafo.
 *
 * Espera-se que sejam inseridos `vertex_capacity` vértices,
 * mas podem ser inseridos mais.
 */
graph* graph_new(size_t vertex_capacity);


/* Adiciona um vértice no grafo e retorna seu valor.
*/
vertex graph_add_vertex(graph* g);
/* Retorna uma estrutura de dados auxiliar que permite buscas rápidas sobre ligações de um nó
*/
vertex_data* graph_get_vertex_data(graph* g, vertex v);
list* graph_get_vertices(graph* g);

bool graph_add_edge(graph* g, vertex v, vertex u, weight w);
bool graph_remove_edge(graph* g, vertex v, vertex u);

list* graph_shortest_paths(graph* g, int k, vertex source, vertex dest);

edge* vd_get_edge(vertex_data* vdata, vertex u); 
list* vd_get_edges(vertex_data* vdata); 
vertex vd_get_id(vertex_data* vdata); 

void graph_free(graph* g);
