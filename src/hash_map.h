#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAP_CAPACITY 100
#define ENOMEM 12
#define ENOENT 2

/** Entry in map, containing a key, hash and value */
typedef struct _hash_map_entry {
	int hash;
	char key[256];
	char value[256];
} hash_map_entry;

/** Hash map defined as array of entries */
typedef struct _hash_map {
	int entries_no;

	hash_map_entry **entries;
} hash_map;

/**
 * init_map() - This function is called to
 * allocate memory for a hash_map.
 *
 * @return: pointer to the allocated map
 */
hash_map *init_map(void);

/**
 * get_hash() - This function is called to
 * get a hash code for an entry.
 *
 * @value: value of the new map entry
 *
 * @return: hash code
 */
int get_hash(char *value);

/**
 * put() - This function is called to
 * add an entry to the map.
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 *
 * @map: address of map object
 * @key: key of the entry
 * @value: value of the entry
 *
 * @return: error code
 */
int put(hash_map *map, char *key, char *value);

/**
 * get() - This function is called to
 * get an entry from the map.
 *
 * @map: address of map object
 * @key: key of the entry
 *
 * @return: value associated with key
 */
char *get(hash_map *map, char *key);

/**
 * remove_entry() - This function is called to
 * remove an entry from the map.
 *
 * @map: address of map object
 * @key: key of the entry
 */
void remove_entry(hash_map *map, char *key);

/**
 * print_map() - This function is called to
 * print all entries of a hash map.
 *
 * @map: address of map object
 */
void print_map(hash_map *map);

/**
 * free_map() - This function is called to
 * free the alocated memory for a map.
 *
 * @map: address of map object
 */
void free_map(hash_map *map);

#endif
