#include "hash_map.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

hash_map* init_map()
{
    hash_map *new_map = (hash_map*)malloc(sizeof(struct _hash_map));
    new_map->entries_no = 0;
    new_map->entries = (hash_map_entry**)malloc(MAP_CAPACITY * sizeof(hash_map_entry*));
    for (int i = 0; i < MAP_CAPACITY; i++) {
        new_map->entries[i] = NULL;
    }

    return new_map;
}

/*http://www.cse.yorku.ca/~oz/hash.html*/
int get_hash(char* value)
{
    int hash = 5381;
    int size = strlen(value);
    
    for (int i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + (value[i] - '0');
    }

    return hash % MAP_CAPACITY;
}

void put(hash_map* map, char* key, char* value)
{
    int index = get_hash(key);

    if (map->entries[index] == NULL) {
        map->entries_no++;
        /* Create new entry and allocate space for it */
        map->entries[index] = (hash_map_entry*)malloc(sizeof(struct _hash_map_entry));
        hash_map_entry* new_entry = map->entries[index];

        int key_size = strlen(key) + 1;
        int value_size = strlen(value) + 1;
        new_entry->key = (char*)malloc(key_size * sizeof(char));
        new_entry->value = (char*)malloc(value_size * sizeof(char));
        strncpy(new_entry->key, key, key_size);
        strncpy(new_entry->value, value, value_size);

    } else {
        /* Updarte old entry */
        hash_map_entry* old_entry = map->entries[index];
        int value_size = strlen(value) + 1;
        free(old_entry->value);
        old_entry->value = (char*)malloc(value_size * sizeof(char));
        strncpy(old_entry->value, value, value_size);
    }
}

char* get(hash_map* map, char* key)
{
    int index = get_hash(key);
    hash_map_entry* target_entry = map->entries[index];
    if (target_entry != NULL)
        return target_entry->value;
    else
        return NULL;
}

void free_map(hash_map* map)
{
    for (int i = 0; i < MAP_CAPACITY; i++) {
        hash_map_entry* entry = map->entries[i];
        if (entry != NULL) {
            free(entry->key);
            free(entry->value);
            free(entry);
        }
    }
    free(map->entries);
    free(map);
}