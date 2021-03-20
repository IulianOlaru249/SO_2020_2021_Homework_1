#ifndef __UTILS_H__
#define __UTILS_H__

#include <ctype.h>
#include "hash_map.h"

#define MAX_LINE_SIZE 256
#define MAX_FILE_LINES 200
#define MAX_FILE_SIZE MAX_FILE_LINES * MAX_LINE_SIZE
#define MAX_INPUT_FILES 100

/**/
int handle_D_flag(hash_map* map, char* param, char** argv, int* i);

/**/
void handle_I_flag(char*** in_file_dirs, int* in_file_dir_no, char* param, char** argv, int* i);

/**/
void handle_o_flag(char* out_file_name, char*param, char**argv, int* i);

/**/
int handle_flags(hash_map* map,
        char*** in_file_names, int* in_file_no,
        char*** in_file_dirs, int* in_file_dir_no,
        char* out_file_name,
        char** argv, int argc);

/**/
int process_input_file(hash_map* map,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);

/**/
int process_line(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);

/**/
int handle_define_directive(hash_map* map, FILE* in_file, char* line);

/**/
void replace_defines(hash_map* map, char* line);

/**/
void handle_undef_directive(hash_map* map, char* line);

/**/
int handle_include_directive(hash_map* map, char* line, 
        FILE* in_file, char* in_file_name, 
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file );

/**/
int handle_if_directive(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);

/**/
int handle_ifdef_directive(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);

/**/
int handle_ifndef_directive(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);
/**/
int handle_false_branch(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);
/**/
int handle_true_branch(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file);

#endif