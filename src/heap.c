#include "heap.h"
#include <stdlib.h>

struct heap {
    list list;
};

typedef struct {
    weight w;
    char ptr[];
} slot;

// Cria e retorna uma fila de prioridade.
heap* heap_new(size_t element_size) {
    return (heap*)list_new(element_size + sizeof(slot));
}
// Cria e retorna uma fila de prioridade a partir de um vetor;

void heap_push(heap* h, void* element, weight w) {
    int index = 0;

    slot* current = list_get(&h->list, index);
    while(!(current && current->w)) {
        index <<= 1;
        if(current->w > w) {
            index += 1;
        }
    }
}
void* heap_pop(heap* h);

void heap_free(heap* h);
