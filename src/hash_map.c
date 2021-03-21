#include "hash_map.h"

/**
 * init_map() - This function is called to
 * allocate memory for a hash_map.
 *
 * @return: pointer to the allocated map
 */
hash_map *init_map(void)
{
	int i = 0;
	hash_map *new_map = NULL;

	new_map = (hash_map *)malloc(sizeof(struct _hash_map));

	if (new_map != NULL) {
		new_map->entries_no = 0;
		new_map->entries = NULL;
		new_map->entries = (hash_map_entry **)malloc(MAP_CAPACITY * sizeof(hash_map_entry *));
		if (new_map->entries != NULL)
			for (; i < MAP_CAPACITY; i++)
				new_map->entries[i] = NULL;
	}
	return new_map;
}

/**
 * get_hash() - This function is called to
 * get a hash code for an entry.
 *
 * @value: value of the new map entry
 *
 * @return: hash code
 */
int get_hash(char *value)
{
	int hash = 0;
	int c;

	while ((c = *value++))
		hash += c;


	return hash % MAP_CAPACITY;
}

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
int put(hash_map *map, char *key, char *value)
{
	int err_code = 0;
	int index = 0;
	int key_size = 0;
	int value_size = 0;
	hash_map_entry *new_entry = NULL;
	hash_map_entry *old_entry = NULL;

	/* get rid of trailing newline */
	if (key[strlen(key) - 1] == '\n')
		key[strlen(key) - 1] = '\0';

	index = get_hash(key);

	if (map->entries[index] == NULL) {
		/* Create new entry and allocate space for it */
		map->entries_no++;
		map->entries[index] = NULL;
		map->entries[index] = (hash_map_entry *)malloc(sizeof(struct _hash_map_entry));
		if (map->entries[index] == NULL) {
			err_code = ENOMEM;
		} else {
			new_entry = map->entries[index];
			key_size = strlen(key) + 1;
			value_size = strlen(value) + 1;

			/* If all went well */
			if (err_code == 0) {
				strncpy(new_entry->key, key, key_size);
				strncpy(new_entry->value, value, value_size);
			}
		}
	} else {
		/* Updarte old entry */
		old_entry = map->entries[index];
		value_size = strlen(value) + 1;

		/* If all went well */
		if (err_code == 0)
			strncpy(old_entry->value, value, value_size);
	}

	return err_code;
}

/**
 * get() - This function is called to
 * get an entry from the map.
 *
 * @map: address of map object
 * @key: key of the entry
 *
 * @return: value associated with key
 */
char *get(hash_map *map, char *key)
{

	int index = -1;
	hash_map_entry *target_entry = NULL;

	/* Look for target */
	index = get_hash(key);
	target_entry = map->entries[index];

	if (target_entry != NULL)
		return target_entry->value;
	else
		return NULL;
}

/**
 * remove_entry() - This function is called to
 * remove an entry from the map.
 *
 * @map: address of map object
 * @key: key of the entry
 */
void remove_entry(hash_map *map, char *key)
{
	hash_map_entry *entry = NULL;
	int index = 0;

	index = get_hash(key);
	if (map->entries[index] == NULL) {
		return;
	} else {
		entry = map->entries[index];
		free(entry);
		map->entries[index] = NULL;
	}
}

/**
 * free_map() - This function is called to
 * free the alocated memory for a map.
 *
 * @map: address of map object
 */
void free_map(hash_map *map)
{
	int i = 0;
	hash_map_entry *entry = NULL;

	if (map == NULL)
		return;

	for (; i < MAP_CAPACITY && map->entries != NULL; i++) {
		entry = map->entries[i];
		if (entry != NULL)
			free(entry);
	}

	if (map->entries != NULL)
		free(map->entries);

	free(map);
}

/**
 * print_map() - This function is called to
 * print all entries of a hash map.
 *
 * @map: address of map object
 */
void print_map(hash_map *map)
{
	hash_map_entry *entry = NULL;
	int i = 0;

	printf("\nMAP ENTRIES:\n-------------------\n");
	printf("%d\n", map->entries_no);
	for (; i < MAP_CAPACITY; i++) {
		entry = map->entries[i];
		if (entry != NULL)
			printf("%s-->%s\n", entry->key, entry->value);
	}
	printf("-------------------\n\n");
}
