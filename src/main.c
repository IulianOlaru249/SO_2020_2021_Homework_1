#include <stdio.h>
#include "hash_map.h"
#include "utils.h"

//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./so-cpp -Itest/dir_1 -I tets/_dir2/subdir -DAia=e -D ana=0 ../so-assignments/1-multi/checker/multi/_test/inputs/test9.in -o ./test.out


int main (int argc, char **argv) {
    hash_map* map = init_map();
    int in_file_no = 0;
    int in_file_dir_no = 0;
    char** in_file_dirs = (char**)malloc(MAX_INPUT_FILES * sizeof(char*));
    char** in_file_names = (char**)malloc(MAX_INPUT_FILES * sizeof(char*));
    char* out_file_name = NULL;
    FILE* out_file = NULL;
    char processed_file[MAX_FILE_SIZE];
    FILE* in_file = NULL;
    int i = 0;
    int invalid_input_files = 0;
    int exit_code = 0;

    handle_flags(map, &out_file_name,
            &in_file_dirs, &in_file_dir_no,
            &in_file_names, &in_file_no,
            argc, argv);

    /**
     * For each input file received as parameter, try open it
     * and process it if it is valid.
     */
    strncpy(processed_file, "\0", 1);
    for(; i < in_file_no; i++) {
        in_file = fopen(in_file_names[i], "r");
        if (strcmp("out", in_file_names[i] + strlen(in_file_names[i]) - 3) == 0) {
            if (out_file_name == NULL) {
                out_file_name = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
            }
            strcpy(out_file_name, in_file_names[i]);       
        } else {
            if (in_file != NULL) {
                exit_code = handle_input_files(processed_file, map, in_file, in_file_names[0], in_file_dirs, in_file_dir_no);
                fclose(in_file);
            } else {
                invalid_input_files = 1;
                exit_code = 1;
                break;
            }
        }
    }

    /* If there was no error opening the input files */
    if (!invalid_input_files && !exit_code) {
        /**
         * When all input files are invalid or there were no input files
         * to open, get the input from stdin.
         */
        if (in_file_no == 0) {
            handle_input_files(processed_file, map, stdin, "./\0", in_file_dirs, in_file_dir_no);
        }

        /* Write to the output file */
        if (out_file_name != NULL) {
            out_file = fopen(out_file_name, "w");
            if (out_file != NULL) {
                fprintf(out_file, "%s", processed_file);
                fclose(out_file);
            } else {
                exit_code = 1;
            }
            free(out_file_name);
        } else {
            fprintf(stdout, "%s", processed_file);
        }
    }

    free(in_file_names);
    free(in_file_dirs);
    free_map(map);
    return exit_code;   
}