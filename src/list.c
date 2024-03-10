#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


list* list_new(size_t element_size) {
    list* l = calloc(sizeof(list), 1);
    l->element_size = element_size;
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
        l->cap <<= 1;
        l->elements = realloc(l->elements, l->cap);
    }

    // Checa se o elemento ficará no meio do vetor
    if (index < l->len) {
        // Se sim, move os elementos a partir de indice para frente.
        size_t bytes_moved = l->element_size * (l->len - index);
        memmove(address_of(l, index), address_of(l, index+1), bytes_moved);
    } 
    // Copia o elemento para dentro do vetor, na posição especificada.
    memcpy(&l->elements[index], element, l->element_size);
    return true;
}

void list_free(list* l) {
    if (l->elements) {
        free(l);
    }
    free(l);
}
