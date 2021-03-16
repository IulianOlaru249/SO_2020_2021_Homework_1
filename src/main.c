#include <stdio.h>
#include "hash_map.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_SIZE 256
#define MAX_INPUT_FILES 100

void handle_D_flag(hash_map* map, char* param, char** argv, int i)
{
    /* Parce definition members */
    char* def = "";
    char* key = NULL;
    char* value = NULL;

    if (param[2] == '\0') {
        def = argv[i + 1];
        /**
         * Skip the next param, since it is the value
         * passed with the flag
         */
        i++;
    } else {
        def = param + 2;
    }

    key = strtok(def, "=");
    value = strtok(NULL, "=");

    /* Store definition */
    if (value != NULL)
        put(map, key, value);
    else
        put(map, key, "");
}

void handle_I_flag(char*** in_file_names, int* in_file_no, char* param, char** argv, int i)
{
    /* Get the pointer to the file name */
    char* aux = NULL;
    if (param[2] == '\0') {
        aux = argv[i + 1];
    } else {
        aux = param + 2;
    }

    (*in_file_names)[*in_file_no] = aux;
    *in_file_no = *in_file_no + 1;
}

void handle_o_flag(char** out_file_name, char*param, char**argv, int i)
{
    if (param[2] == '\0') {
        *out_file_name = argv[i + 1];
    } else {
        *out_file_name = param + 2;
    }
}

void handle_flags(hash_map* map, 
        char** out_file_name, char*** in_file_names,
        int* in_file_no, int argc, char** argv)
{
    int i = 0;
    for (; i < argc; i++) {
        char* param = argv[i];
        /* If flag detected */
        if (param[0] == '-') {
            if (param[1] == 'D') {
                /* Store definition in map */
                handle_D_flag(map, param, argv, i);
            } else if (param[1] == 'I') {
                /* Save the path to input files */
                handle_I_flag(in_file_names, in_file_no, param, argv, i);
            } else if (param[1] == 'o') {
                /* Save the name of the output file */
                handle_o_flag(out_file_name, param, argv, i);
            }
        }
    }
}

int main (int argc, char **argv) {
    hash_map* map = init_map();
    int in_file_no = 0;
    char** in_file_names = (char**)malloc(MAX_INPUT_FILES * sizeof(char*));
    char* out_file_name = NULL;
    FILE* in_file = NULL;
    char line[MAX_LINE_SIZE];
    int i = 0;

    handle_flags(map, &out_file_name, &in_file_names, &in_file_no, argc, argv);

    /* For each input file received as parameter */
    for(; i < in_file_no; i++) {
        char key[MAX_LINE_SIZE];
        char value[MAX_LINE_SIZE];
        int white_space_counter = 0;

        in_file = fopen(in_file_names[i], "r");
        /* For each line in the file */
        while (fgets(line, MAX_LINE_SIZE, in_file)) {
            /* Handle #define keywords */
            if (strncmp(line, "#define", 6) == 0) {
                strtok(line, " ");
                strcpy(key, strtok(NULL, " "));
                strcpy(value, strtok(NULL, "\n"));

                /* While processing a multi-line define */
                while (value[strlen(value) - 1] == '\\') {
                    /* Get rid of thailing whitespace or 'whitespace + backslash' or backslash */
                    if(value[strlen(value) - 2] == ' ')
                        value[strlen(value) - 2] = '\0';
                    else
                        value[strlen(value) - 1] = '\0';

                    /* gen the next line from the define, process it and append to value */
                    if(!fgets(line, MAX_LINE_SIZE, in_file))
                        break;

                    white_space_counter = 0;
                    while(isspace((unsigned char)line[white_space_counter])) white_space_counter++;
                    line[strlen(line) - 1] = '\0';
                    strcat(value, line + white_space_counter - 1);
                }
                
                put(map, key, value);
            }
        }
        fclose(in_file);
    }

    printf("\nMAP ENTRIES:\n-------------------\n");
    i = 0;
    for (; i < MAP_CAPACITY; i++) {
        hash_map_entry* entry = map->entries[i];
        if (entry != NULL) {
            printf("%s-->%s\n", entry->key, entry->value);
        }
    }

    //printf("ASDFSDFAF: %d == %d\n", get_hash("BCD"), get_hash("VAR0"));

    free(in_file_names);
    free_map(map);
    return 0;
}