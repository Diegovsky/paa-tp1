#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static void list_resize(list* l, size_t new_element_count) {
    l->cap = new_element_count;
    l->elements = realloc(l->elements, l->cap * l->element_size);
}

list* list_new(size_t element_size) {
    list* l = calloc(sizeof(list), 1);
    l->element_size = element_size;
    list_resize(l, 4);
    return l;
}

list* list_new_from(size_t element_size, void* data, size_t element_count) {
    list* l = malloc(sizeof(list));
    l->element_size = element_size;
    l->len = element_count;

    list_resize(l, element_count);
    memcpy(l->elements, data, element_count * element_size);

    return l;
}

void list_push(list* l, void* element) {
    assert(list_insert(l, element, l->len));
}

static inline void* address_of(list* l, size_t index) {
    return l->elements + l->element_size * index;
}

bool list_insert(list* l, void* element, size_t index) {
    // Caso o índice esteja fora do vetor, a operaçã́o falha.
    if (index > l->len) {
        return false;
    }
    // Checa se estamos sem capacidade para mais elementos.
    if (l->cap <= l->len) {
        // Se sim, dobra a capacidade e realoca a memória
        // com a nova capacidade.
        list_resize(l, l->cap<<1);
    }

    // Checa se o elemento ficará no meio do vetor
    if (index < l->len) {
        // Se sim, move os elementos a partir de indice para frente.
        size_t bytes_moved = l->element_size * (l->len - index);
        memmove(address_of(l, index+1), address_of(l, index), bytes_moved);
    } 
    // Copia o elemento para dentro do vetor, na posição especificada.
    memcpy(address_of(l, index), element, l->element_size);
    l->len++;
    return true;
}

void* list_get(list* l, size_t index) {
    // Caso o índice esteja fora do vetor, retorna NULL;
    if (index > l->len) {
        return NULL;
    }
    return address_of(l, index);
}

void list_free(list* l) {
    if (l->elements) {
        free(l->elements);
    }
    free(l);
}
