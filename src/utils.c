#include "utils.h"

void handle_D_flag(hash_map* map, char* param, char** argv, int* i)
{
    /* Parce definition members */
    char* def = "";
    char* key = NULL;
    char* value = NULL;

    if (param[2] == '\0') {
        def = argv[*i + 1];
        /**
         * Skip the next param, since it is the value
         * passed with the flag
         */
        *i = *i + 1;
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

void handle_I_flag(char*** in_file_dirs, int* in_file_dir_no, char* param, char** argv, int* i)
{
    /* Get the pointer to the file name */
    char* aux = NULL;
    if (param[2] == '\0') {
        aux = argv[*i + 1];
        *i = *i + 1;
    } else {
        aux = param + 2;
    }

    (*in_file_dirs)[*in_file_dir_no] = aux;
    *in_file_dir_no = *in_file_dir_no + 1;
}

void handle_o_flag(char** out_file_name, char*param, char**argv, int* i)
{
    if (param[2] == '\0') {
        *out_file_name = argv[*i + 1];
        *i = *i + 1;
    } else {
        *out_file_name = param + 2;
    }
}

void handle_flags(hash_map* map, char** out_file_name,
        char*** in_file_dirs, int* in_file_dir_no,
        char*** in_file_names, int* in_file_no,
        int argc, char** argv)
{
    int i = 1;
    for (; i < argc; i++) {
        char* param = argv[i];
        /* If flag detected */
        if (param[0] == '-') {
            if (param[1] == 'D') {
                /* Store definition in map */
                handle_D_flag(map, param, argv, &i);
            } else if (param[1] == 'I') {
                /* Save the path to input files */
                handle_I_flag(in_file_dirs, in_file_dir_no, param, argv, &i);
            } else if (param[1] == 'o') {
                /* Save the name of the output file */
                handle_o_flag(out_file_name, param, argv, &i);
            }
        } else {
            (*in_file_names)[*in_file_no] = param;
            *in_file_no = *in_file_no + 1;
        }
    }
}

void handle_define_directive(hash_map* map, FILE* in_file, char* line)
{
    char key[MAX_LINE_SIZE];
    char value[MAX_LINE_SIZE];
    int white_space_counter = 0;
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

        /* Get the next line from the define, process it and append to value */
        if(!fgets(line, MAX_LINE_SIZE, in_file))
            break;

        white_space_counter = 0;
        while(isspace((unsigned char)line[white_space_counter])) white_space_counter++;
        line[strlen(line) - 1] = '\0';
        strcat(value, line + white_space_counter - 1);
    }

    put(map, key, value);
}

void handle_undef_directive(hash_map* map, char* line)
{
    char value[MAX_LINE_SIZE];
    strtok(line, " ");
    strcpy(value, strtok(NULL, "\n"));

    if(get(map, value) != NULL) {
        remove_entry(map, value);
    }
}

void handle_ifelse_directive(hash_map* map, FILE* in_file, char* line)
{
    char new_line[MAX_LINE_SIZE];
    char value[MAX_LINE_SIZE];
    strtok(line, " ");
    strcpy(value, strtok(NULL, "\n"));

    if(get(map, value) != NULL) {
        replace_defines(map, value);
        if (strcmp(value, "") == 0) {
            strncpy(value, "1\0", 2);
        }
    }

    /* If condition is not met */
    if (atoi(value) == 0) {
        /**
         *  Skip over lines as long as no endif is found
         *  or no else or elseif branch is found.
         */
        while (fgets(line, MAX_LINE_SIZE, in_file)
                && strncmp(line, "#endif", 6) != 0) {

            if(strncmp(line, "#elif", 5) == 0) {
                /* Get the condition */
                strtok(line, " ");
                strcpy(value, strtok(NULL, "\n"));
                /* evaluate condition */
                if(get(map, value) != NULL) {
                    replace_defines(map, value);
                }

                /** 
                 * If the condition is true copy the code block
                 * to the line, else don't care. Keep looking for
                 * #else statements
                 */
                if (atoi(value) != 0) {
                    strncpy(line, "\0", 1);
                    while (fgets(new_line, MAX_LINE_SIZE, in_file) &&
                            strncmp(new_line, "#endif", 6) != 0) {
                        
                        if (strncmp(new_line, "#elif", 5) == 0 ||
                                strncmp(new_line, "#else", 5) == 0) {
                            /* Skip till #endif statement */
                            while (fgets(new_line, MAX_LINE_SIZE, in_file) &&
                            strncmp(new_line, "#endif", 6) != 0);
                            break;
                        }

                        if (strncmp(line, "\n", 1) != 0) {
                            strcat(line, new_line);
                        }
                    }
                    break;
                }
            }

            if(strncmp(line, "#else", 5) == 0) {
                strncpy(line, "\0", 1);
                while (fgets(new_line, MAX_LINE_SIZE, in_file)
                        && strncmp(new_line, "#endif", 6) != 0) {
                    if (strncmp(line, "\n", 1) != 0) {
                        strcat(line, new_line);
                    }
                }
                break;
            }

        }
    } else {
        /* If condition is met */
        /* Empty the line */
        strncpy(line, "\0", 1);
        /**
         * Append to the line everything that
         * is not in an else branch
         */
        while (fgets(new_line, MAX_LINE_SIZE, in_file) &&
                strncmp(new_line, "#endif", 6) != 0 ) {

            /* Skip everything that is in an else or elif branch */
            if (strncmp(new_line, "#else", 5) == 0 ||
                    strncmp(new_line, "#elif", 5) == 0) {
                while (fgets(new_line, MAX_LINE_SIZE, in_file)
                    && strncmp(new_line, "#endif", 6) != 0);
            } else {
                if (strncmp(line, "\n", 1) != 0) {
                    strcat(line, new_line);
                }
            }
        }
    }
}

void replace_defines(hash_map* map, char line[MAX_LINE_SIZE])
{
    char* aux = NULL;
    for(int i = 0; i < MAP_CAPACITY; i++) {
        hash_map_entry* entry = map->entries[i];
        char* word = line;
        char* open_quotation = NULL;
        char* closed_quotation = NULL;
        int inside_quotes = 0;
        /* Search for pos of open and closed quotes */
        open_quotation = strstr(line, "\"");
        if (open_quotation != NULL) {
            closed_quotation = strstr(open_quotation + 1, "\"");
        }

        if (entry != NULL) {
            while (word != NULL) {
                word = strstr(word, entry->key);
                inside_quotes = 0;
                if (word != NULL) {
                    if ((open_quotation != NULL && closed_quotation != NULL) &&
                            (open_quotation < word && closed_quotation > word)) {
                        inside_quotes = 1;
                    }

                    if (!inside_quotes) {
                        aux = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
                        strcpy(aux,  word + strlen(entry->key));
                        int i = 0;
                        for (; i < strlen(entry->value); i++) {
                            *(word + i) = entry->value[i];
                        }
                        *(word + i) = '\0';
                        strcat(line, aux);
                        free(aux);
                    } else {
                        word = word + 1;
                    }
                }
            }
        }
    }
}

void handle_ifndef_directive(hash_map* map, char*line, FILE* in_file, char* processed_file)
{
    char value[MAX_LINE_SIZE];
    strtok(line, " ");
    strcpy(value, strtok(NULL, "\n"));

    if(get(map, value) == NULL) {
        while (fgets(line, MAX_LINE_SIZE, in_file)
                && strncmp(line, "#endif", 6) != 0) {
            if (strncmp(line, "#define", 6) == 0) {
                /* Handle #define keywords */
                handle_define_directive(map, in_file, line);
            } else if (strncmp(line, "#undef", 6) == 0) {
                /* Handle #undef keywords */
                handle_undef_directive(map, line);
            }

            if (strncmp(line, "#undef", 6) != 0 &&
                    strncmp(line, "#define", 7) != 0) {
                strcat(processed_file, line);
            }
        }
    } else {
        while (fgets(line, MAX_LINE_SIZE, in_file)
            && strncmp(line, "#endif", 6) != 0);
    }
}

void handle_ifdef_directive(hash_map* map, char*line, FILE* in_file, char* processed_file)
{
    char value[MAX_LINE_SIZE];
    strtok(line, " ");
    strcpy(value, strtok(NULL, "\n"));

    if(get(map, value) != NULL) {
        while (fgets(line, MAX_LINE_SIZE, in_file)
                && strncmp(line, "#endif", 6) != 0) {
            //printf("%s\n", line);
            if (strncmp(line, "#define", 6) == 0) {
                /* Handle #define keywords */
                handle_define_directive(map, in_file, line);
            } else if (strncmp(line, "#undef", 6) == 0) {
                /* Handle #undef keywords */
                handle_undef_directive(map, line);
            }

            if (strncmp(line, "#undef", 6) != 0 &&
                    strncmp(line, "#define", 7) != 0) {
                strcat(processed_file, line);
            }
        }
    } else {
         while (fgets(line, MAX_LINE_SIZE, in_file)
                && strncmp(line, "#endif", 6) != 0);
    }
}

int handle_include_directive(char* processed_file, hash_map* map,
    FILE* in_file, char* in_file_name, char* line,
    char** in_file_dirs, int in_file_dir_no)
{
    int error_code = 0;
    FILE* include_file = NULL;
    char path[MAX_LINE_SIZE] = {};
    char *include_file_name;
    int i = 0;

    strncpy(path, in_file_name, strlen(in_file_name) - 9);
    strtok(line, " ");
    include_file_name = strtok(NULL, "\n");
    include_file_name[strlen(include_file_name) - 1] = '\0';

    /* Look for the include file in the path of the input file */
    strcat(path, include_file_name + 1);
    include_file = fopen(path, "r");
    if (include_file == NULL) {
        /* Look for the include file in all other paths received as params */
        for (i = 0; i < in_file_dir_no; i++) {
            strcat(in_file_dirs[i], "/");
            include_file = fopen(strcat(in_file_dirs[i], include_file_name + 1), "r");
            if(include_file != NULL) {
                break;
            }
        }
    }
    
    if (include_file != NULL ){
        error_code = handle_input_files(processed_file, map, include_file, include_file_name, in_file_dirs, in_file_dir_no);
        fclose(include_file);
    } else {
        error_code = 1;
    }

    return error_code;
}


int handle_input_files(char* processed_file, hash_map* map,
        FILE* in_file, char* in_file_name, char** in_file_dirs, int in_file_dir_no)
{
    char line[MAX_LINE_SIZE];
    int err_code = 0;
    /* For each line in the file */
    while (fgets(line, MAX_LINE_SIZE, in_file)) {
        if (strncmp(line, "#define", 6) == 0) {
            /* Handle #define keywords */
            handle_define_directive(map, in_file, line);
        } else if (strncmp(line, "#include", 8) == 0) {
            /* Handle #include keywords */
            err_code = handle_include_directive(processed_file, map, in_file, in_file_name, line, in_file_dirs, in_file_dir_no);
        } else if (strncmp(line, "#undef", 6) == 0) {
            /* Handle #undef keywords */
            handle_undef_directive(map, line);
        } else if (strncmp(line, "#ifndef", 7) == 0) {
            /* Handle #ifndef keywords */
            handle_ifndef_directive(map, line, in_file, processed_file);
        } else if (strncmp(line, "#ifdef", 6) == 0) {
            /* Handle #ifdef keywords */
            handle_ifdef_directive(map, line, in_file, processed_file);
        } else if (strncmp(line, "#if", 3) == 0) {
            /* Handle #if keywords */
            handle_ifelse_directive(map, in_file, line);
            /* Print the line only if not #endif directive */
            if ( strncmp(line, "#endif", 6) != 0) {
                strcat(processed_file, line);
            }
        } else if (strncmp(line, "\n", 1) != 0 &&
                strncmp(line, "#endif", 6) != 0){
            replace_defines(map, line);
            strcat(processed_file, line);
        }
    }
    return err_code;
}
