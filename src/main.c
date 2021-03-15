#include <stdio.h>
#include "hash_map.h"

int main (void) {
    hash_map *map = init_map();

    char *target = NULL;
    
    put(map, "A", "99");
    target = get(map, "A");
    printf("%s\n", target);

    put(map, "A", "98");
    target = get(map, "A");
    printf("%s\n", target);

    put(map, "B", "101");
    target = get(map, "B");
    printf("%s\n", target);

    put(map, "Banana", "adasd");
    target = get(map, "Banana");
    printf("%s\n", target);


    target = get(map, "asdB");
    if (target != NULL) {
       printf("%s\n", target);
    }

    free_map(map);
    return 0;
}