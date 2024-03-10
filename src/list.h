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

void list_push(list* l, void* element);
bool list_insert(list* l, void* element, size_t index);

#define list_elements(l, type) ((type*)l->elements)

void list_free(list* l);
