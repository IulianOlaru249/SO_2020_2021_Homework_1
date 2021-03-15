#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#define MAP_CAPACITY 100

/**  */
typedef struct _hash_map_entry {
    int hash;
    char* key;
    char* value;
} hash_map_entry;

/**  */
typedef struct _hash_map {
    int entries_no;
    hash_map_entry **entries;
} hash_map;

hash_map* init_map();
int get_hash(char* value);
void put(hash_map* map, char* key, char* value);
char* get(hash_map* map, char* key);
void free_map(hash_map* map);

#endif
