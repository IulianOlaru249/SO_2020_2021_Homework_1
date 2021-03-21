#include "hash_map.h"
#include "utils.h"

int main(int argc, char **argv)
{
	/* Hash map objects for defines */
	hash_map *map = NULL;

	/* Variables to handle input files */
	int in_file_no = 0;
	char **in_file_names = NULL;
	FILE *in_file = NULL;

	/* Variables to handle directories for included files */
	int in_file_dir_no = 0;
	char **in_file_dir_names = NULL;

	/* Variables to handle output file */
	char *out_file_name = NULL;
	FILE *out_file = NULL;

	/* Misc variables */
	char processed_file[MAX_FILE_SIZE] = {0};
	int exit_code = 0;
	int i = 0;

	/* Initializations */
	map = init_map();
	if (map == NULL)
		exit_code = ENOMEM;

	in_file_dir_names = (char **)malloc(MAX_INPUT_FILES * sizeof(char *));
	if (in_file_dir_names == NULL)
		exit_code = ENOMEM;

	in_file_names = (char **)malloc(MAX_INPUT_FILES * sizeof(char *));
	if (in_file_names == NULL)
		exit_code = ENOMEM;

	out_file_name = (char *)calloc(MAX_LINE_SIZE, sizeof(char));
	if (out_file_name == NULL)
		exit_code = ENOMEM;

	/* Handle the command line params if initialization went fine */
	if (exit_code == 0) {
		exit_code = handle_flags(map,
				&in_file_names, &in_file_no,
				&in_file_dir_names, &in_file_dir_no,
				out_file_name,
				argv, argc);
	}

	/* Handle the input files if the params were handled fine */
	if (exit_code == 0) {
		for (i = 0; i < in_file_no; i++) {
			/* If output files were passed as params
			 * (for whatever reason)
			 */
			if (strncmp("out", in_file_names[i] +
					strlen(in_file_names[i]) - 3, 3) == 0) {
				strncpy(out_file_name, in_file_names[i], MAX_LINE_SIZE);
			} else {
				/* If the specified file is an input file */
				in_file = fopen(in_file_names[i], "r");
				/* If is not a valid file */
				if (in_file != NULL) {
					/* If it was a valid file, process it */
					exit_code = process_input_file(map,
						in_file, in_file_names[i],
						in_file_dir_names, in_file_dir_no,
						processed_file);

					fclose(in_file);
				} else {
					exit_code = ENOENT;
					break;
				}
			}
		}
	}

	/* Handle the output if the file was processed fine */
	if (exit_code == 0) {
		/* If there was no input file sp[ecified, take input from stdin */
		if (in_file_no == 0) {
			exit_code = process_input_file(map,
				stdin, "./\0",
				in_file_dir_names, in_file_dir_no,
				processed_file);
		}

		/* If the out file name was specified */
		if (out_file_name != NULL &&
				strncmp(out_file_name, "\0", 1) != 0) {
			/* If an out file was specified */
			out_file = fopen(out_file_name, "w");
			/* If out file fails to open */
			if (out_file == NULL) {
				exit_code = ENOENT;
			} else {
				fprintf(out_file, "%s", processed_file);
				fclose(out_file);
			}
		} else {
			/* If no out file was specified, print to stdout */
			fprintf(stdout, "%s", processed_file);
		}
	}

	/* Free allocated space */
	if (out_file_name != NULL)
		free(out_file_name);

	if (in_file_names != NULL)
		free(in_file_names);

	if (in_file_dir_names != NULL)
		free(in_file_dir_names);

	if (map != NULL)
		free_map(map);

	return exit_code;
}
