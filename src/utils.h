#ifndef __UTILS_H__
#define __UTILS_H__

#include <ctype.h>
#include "hash_map.h"

#define MAX_LINE_SIZE 256
#define MAX_FILE_LINES 200
#define MAX_FILE_SIZE (MAX_FILE_LINES * MAX_LINE_SIZE)
#define MAX_INPUT_FILES 100

/**
 * handle_D_flag() - This function is called When -D flag is encountered.
 * Stores the defined value in the hash map.
 *
 * @map: hash map of definitions
 * @param: command line parameter value
 * @argv: all command line parameters
 * @argc: number of command line parameters
 *
 * @return: error code
 */
int handle_D_flag(hash_map *map, char *param, char **argv, int *i);

/**
 * handle_I_flag() - This function is called when -I flag is encountered.
 * Stores the paths to the input directories in memory.
 *
 * @in_file_dirs: address of array to keep track of paths
 * @in_file_dir_no: address of number of paths
 * @param: command line parameter value
 * @argv: all command line parameters
 * @i: index of current parameter
 */
void handle_I_flag(char ***in_file_dirs, int *in_file_dir_no, char *param, char **argv, int *i);

/**
 * handle_o_flag() - This function is called when -o flag is encountered.
 * Stores the path to the output file in memory.
 *
 * @out_file_name: path to output file
 * @param: command line parameter value
 * @argv: all command line parameters
 * @i: index of current parameter
 */
void handle_o_flag(char *out_file_name, char *param, char **argv, int *i);

/**
 * handle_flags() - This function is called if there are command line arguments.
 * Decides whics handle to call based on the flags received in the command line.
 *
 * @map: hash map of definitions
 * @in_file_names: address of array to keep track if input files
 * @in_file_no: address if number of paths to input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @out_file_name: path to output file
 * @argv: all command line parameters
 * @argc: number of command line parameters
 *
 * @return: error code
 */
int handle_flags(hash_map *map,
	char ***in_file_names, int *in_file_no,
	char ***in_file_dirs, int *in_file_dir_no,
	char *out_file_name,
	char **argv, int argc);

/**
 * process_input_file() - This function is called in order to process a file
 * line by line and builds the result in a new buffer.
 *
 * @map: hash map of definitions
 * @in_file_names: address of array to keep track if input files
 * @in_file_no: address if number of paths to input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int process_input_file(hash_map *map,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

/**
 * process_line() - This function is called in order to process a line.
 * Decides which handle to call based on the directives it encounters.
 *
 * @map: hash map of definitions
 * @in_file_names: address of array to keep track if input files
 * @in_file_no: address if number of paths to input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int process_line(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

/**
 * handle_define_directive() - This function is called when #define
 * is encountered. Stores the defined value in the map
 *
 * @map: hash map of definitions
 * @in_file: input file stream
 * @line: line to be processed
 *
 * @return: error code
 */
int handle_define_directive(hash_map *map, FILE *in_file, char *line);

/**
 * replace_defines() - This function is called to replace all
 * defined macros from a line.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 */
void replace_defines(hash_map *map, char *line);

/**
 * handle_undef_directive() - This function is called when #undef
 * is encountered. Removes defined macros from map.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 */
void handle_undef_directive(hash_map *map, char *line);

/**
 * handle_include_directive() - This function is called when #include
 * is encountered. If valid, openes the included file and
 * starts to process it.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 * @in_file: input file stream
 * @in_file_names: address of array to keep track if input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int handle_include_directive(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

/**
 * handle_if_directive() - This function is called when #if
 * is encountered. Evaluates the conditions and executes the
 * coresponding branches.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 * @in_file: input file stream
 * @in_file_names: address of array to keep track if input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int handle_if_directive(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

/**
 * handle_ifndef_directive() - This function is called when #if
 * is encountered. Evaluates the conditions and executes the
 * coresponding branches.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 * @in_file: input file stream
 * @in_file_names: address of array to keep track if input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int handle_ifdef_directive(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

/**
 * handle_ifdef_directive() - This function is called when #if
 * is encountered. Evaluates the conditions and executes the
 * coresponding branches.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 * @in_file: input file stream
 * @in_file_names: address of array to keep track if input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int handle_ifndef_directive(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

/**
 * handle_false_branch() - This function is called when a
 * condition was evaluated to False. It goes trough each
 * line of the positive branch, also looking for elses and
 * elifs.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 * @in_file: input file stream
 * @in_file_names: address of array to keep track if input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int handle_false_branch(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);
/**
 * handle_true_branch() - This function is called when a
 * condition was evaluated to True. It goes trough each
 * line of the positive branch.
 *
 * @map: hash map of definitions
 * @line: line to be processed
 * @in_file: input file stream
 * @in_file_names: address of array to keep track if input files
 * @in_file_dirs: address of array to keep track of paths to directories
 * @in_file_dir_no: address of number of paths to directories
 * @processed_file: buffer to keep the processed files
 *
 * @return: error code
 */
int handle_true_branch(hash_map *map, char *line,
	FILE *in_file, char *in_file_name,
	char **in_file_dirs, int in_file_dir_no,
	char *processed_file);

#endif
