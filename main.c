#include <time.h>
#include <stdio.h>
#include "debug.c"
#include "arenas.h"
#include "fastr_hash.h"

int main(int argc, char* argv[]) {
    arena hash_arena = aalloc(16778400);
    u16* hashmap = init_hash_map(hash_arena);
    
    clock_t before = clock();

    FILE* file = fopen(argv[1], "r");
    if (!file) {printf("fail\n"); exit(0);}
    int total = 0;

    while (1) {
        char buff[128] = {0};
        fgets(buff, 127, file);
        int len = strlen(buff);
        if (len == 0) {break;}
        buff[--len] = '\0';
        total++;

        if (!is_in_hashmap(hashmap, buff, len)) {
            hashmap_push(hashmap, buff, len, 1);
        }
    }
    int added_elements = 0;
    for (int i = 0; i < 8388607; i++) {if (hashmap[i] != 0) {added_elements++;}}

    printf("\rCollisions: %d / %d\n", total - added_elements, total);

    clock_t after = clock();

    fflush(stdout);
    printf("\nruntime = %f\n", after - before);
}