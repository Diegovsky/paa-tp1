#include<stdio.h>

#include "list.h"

int main() {
    list* l = list_new(sizeof(int));
    size_t len;
    scanf("%luz", &len);
    for(int i = 0; i < len; i++) {
        int x;
        scanf("%d", &x);
        list_push(l, &x);
    }
    int sum = 0;
    for(int i = 0; i < len; i++) {
        sum += list_elements(l, int)[i];
    }
    printf("Sum: %d\n", sum);
    list_free(l);
    return 0;
}
