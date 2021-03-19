#ifndef __UTILS_H__
#define __UTILS_H__

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "hash_map.h"

#define MAX_LINE_SIZE 256
#define MAX_FILE_SIZE 200 * 256
#define MAX_INPUT_FILES 100

void handle_flags(hash_map* map, char** out_file_name,
        char*** in_file_dirs, int* in_file_dir_no,
        char*** in_file_names, int* in_file_no,
        int argc, char** argv);
void handle_D_flag(hash_map* map, char* param, char** argv, int* i);
void handle_I_flag(char*** in_file_dir_names, int* in_file_dir_no, char* param, char** argv, int* i);
void handle_o_flag(char** out_file_name, char*param, char**argv, int* i);
void handle_define_directive(hash_map* map, FILE* in_file, char* line);
void handle_undef_directive(hash_map* map, char* line);
void handle_ifelse_directive(hash_map* map, FILE* in_file, char* line);
void replace_defines(hash_map* map, char line[MAX_LINE_SIZE]);
int handle_input_files(char* processed_file, hash_map* map, FILE* in_file, char* in_file_path, char** in_file_dirs, int in_file_dir_no);
void handle_ifndef_directive(hash_map* map, char*line, FILE* in_file, char* processed_file);
void handle_ifdef_directive(hash_map* map, char*line, FILE* in_file, char* processed_name);
int handle_include_directive(char* processed_file, hash_map* map, FILE* in_file, char* in_file_name, char* line, char** in_file_dirs, int in_file_dir_no);

#endif