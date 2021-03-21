#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAP_CAPACITY 100
#define ENOMEM 12
#define ENOENT 2
/**  */
typedef struct _hash_map_entry {
    int hash;
    char key[256];
    char value[256];
} hash_map_entry;

/**  */
typedef struct _hash_map {
    int entries_no;
    hash_map_entry **entries;
} hash_map;

hash_map* init_map();
int get_hash(char* value);
int put(hash_map* map, char* key, char* value);
char* get(hash_map* map, char* key);
void remove_entry(hash_map* map, char* key);
void print_map(hash_map* map);
void free_map(hash_map* map);

#endif
