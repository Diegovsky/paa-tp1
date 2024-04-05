#pragma once
#include <stdio.h>
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
list* graph_get_vertices(graph* g);

bool graph_add_edge(graph* g, vertex v, vertex u, weight w);

list* graph_shortest_paths(graph* g, int k, vertex source, vertex dest);

void graph_free(graph* g);

struct tp_info {
    struct graph* g;
    int k;
    int end;
    int start;
};

/*Lê o grafo do arquivo e representa-o com lista de adjacência*/
struct tp_info parse_info(FILE* input_file);
