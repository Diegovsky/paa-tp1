#pragma once

#include <stddef.h>
#include <stdbool.h>

// Uma lista dinâmica de elementos de tamanho fixo.
typedef struct {
    void* elements;
    size_t element_size;
    size_t len;
    size_t cap;
} list;

// Cria e retorna uma lista.
list* list_new(size_t element_size);
// Cria e retorna uma lista a partir de um vetor;
list* list_new_from(size_t element_size, void* data, size_t element_count);

void list_push(list* l, void* element);
bool list_insert(list* l, void* element, size_t index);
void* list_get(list* l, size_t index);

bool list_remove(list* l, size_t index);

typedef int (*element_comparator_fn)(const void*, const void*);
void list_sort(list* l, element_comparator_fn comparator);
bool list_eq(list* l1, list* l2, element_comparator_fn comparator);
void* list_min(list* l, element_comparator_fn comparator);

#define list_foreach(l, type, var)\
    for(type* var = (l)->elements;\
        var < (type*)((l)->elements + (l)->len * (l)->element_size);\
        var = ((void*) var + (l)->element_size))

void list_free(list* l);
