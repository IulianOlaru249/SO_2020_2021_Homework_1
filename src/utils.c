#include "utils.h"

int handle_D_flag(hash_map* map, char* param, char** argv, int* i)
{
    int err_code = 0;

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
    if (value != NULL) {
        err_code = put(map, key, value);
    }
    else {
        err_code = put(map, key, "");
    }

    return err_code;
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

void handle_o_flag(char* out_file_name, char*param, char**argv, int* i)
{
    /* Get pointer to the out file name */
    if (param[2] == '\0') {
        strncpy(out_file_name, argv[*i + 1], MAX_LINE_SIZE);
        //*out_file_name = argv[*i + 1];
        *i = *i + 1;
    } else {
        strncpy(out_file_name, (param + 2), MAX_LINE_SIZE);
        //*out_file_name = param + 2;
    }
}

int handle_flags(hash_map* map,
        char*** in_file_names, int* in_file_no,
        char*** in_file_dirs, int* in_file_dir_no,
        char* out_file_name,
        char** argv, int argc)
{
    int err_code = 0;
    int i = 1;
    for (; i < argc; i++) {
        char* param = argv[i];
        /* If flag detected */
        if (param[0] == '-') {
            if (param[1] == 'D') {
                /* Store definition in map */
                err_code = handle_D_flag(map, param, argv, &i);
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

    return err_code;
}

int process_input_file(hash_map* map,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    char line[MAX_LINE_SIZE] = {0};
    int err_code = 0;

    /* Go trough each line */
    while (fgets(line, MAX_LINE_SIZE, in_file) && err_code == 0) {

        err_code = process_line(map, line,
            in_file, in_file_name,
            in_file_dirs, in_file_dir_no,
            processed_file);

        if (strncmp(line, "\n", 1) != 0 &&
                strncmp(line, "#", 1) != 0){
            /**
             * If line is no directive, or blank replace all
             * definitions and add to final reult.
             */
            replace_defines(map, line);
            strncat(processed_file, line, MAX_FILE_SIZE);
        }
    }

    return err_code;
}

int process_line(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    int err_code = 0;

    if (strncmp(line, "#define", 6) == 0) {
        /* Handle #define keywords */
        err_code = handle_define_directive(map, in_file, line);
    } else if (strncmp(line, "#undef", 6) == 0) {
        /* Handle #undef keywords */
        handle_undef_directive(map, line);
    } else if (strncmp(line, "#include", 8) == 0) {
        /* Handle #include keywords */
        err_code = handle_include_directive(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    } else if (strncmp(line, "#ifndef", 7) == 0) {
        /* Handle #ifdef keywords */
        err_code = handle_ifndef_directive(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    } else if (strncmp(line, "#ifdef", 6) == 0) {
        /* Handle #ifdef keywords */
        err_code = handle_ifdef_directive(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    } else if (strncmp(line, "#if", 3) == 0) {
        /* Handle #if keywords */
        err_code = handle_if_directive(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    }

    return err_code;
}

int handle_define_directive(hash_map* map, FILE* in_file, char* line)
{
    int err_code = 0;
    int multiline = 0;
    char* aux = NULL;
    char key[MAX_LINE_SIZE] = {0};
    char value[MAX_LINE_SIZE] = {0};
    int white_space_counter = 0;

    /* Tokenize the line to get the key and value pair */
    strtok(line, " ");
    strcpy(key, strtok(NULL, " "));
    aux = strtok(NULL, "\n");
    if (aux != NULL) {
        strcpy(value, aux);
    }

    /* While processing a multi-line define */
    while (value[strlen(value) - 1] == '\\') {
        multiline = 1;
        /* Get rid of thailing whitespace or 'whitespace + backslash' or backslash */
        if(value[strlen(value) - 2] == ' ')
            value[strlen(value) - 2] = '\0';
        else
            value[strlen(value) - 1] = '\0';

        /* If EOF */
        if(!fgets(line, MAX_LINE_SIZE, in_file))
            break;

        /* Get the next line from the define, process it and append to value */
        white_space_counter = 0;
        while(isspace((unsigned char)line[white_space_counter])) white_space_counter++;
        line[strlen(line) - 1] = '\0';
        strcat(value, line + white_space_counter - 1);
    }

    if (multiline) {
        /* Move cursor past last line of define */
        fgets(line, MAX_LINE_SIZE, in_file);
    }

    /* Insert pair in map */
    err_code = put(map, key, value);

    return err_code;
}

void handle_undef_directive(hash_map* map, char* line)
{
    char value[MAX_LINE_SIZE] = {0};
    strtok(line, " ");
    strcpy(value, strtok(NULL, "\n"));

    if(get(map, value) != NULL) {
        remove_entry(map, value);
    }
}

void replace_defines(hash_map* map, char* line)
{
    char* word = NULL;
    char* open_quotation = NULL;
    char* closed_quotation = NULL;
    char aux[MAX_LINE_SIZE] = {0};
    int inside_quotes = 0;
    int i = 0;

    for(i = 0; i < MAP_CAPACITY; i++) {
        hash_map_entry* entry = map->entries[i];
        word = line;
        open_quotation = NULL;
        closed_quotation = NULL;
        inside_quotes = 0;
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
                        strcpy(aux,  word + strlen(entry->key));
                        i = 0;
                        for (; i < strlen(entry->value); i++) {
                            *(word + i) = entry->value[i];
                        }
                        *(word + i) = '\0';
                        strcat(line, aux);
                    } else {
                        word = word + 1;
                    }
                }
            }
        }
    }
}

int handle_include_directive(hash_map* map, char* line, 
        FILE* in_file, char* in_file_name, 
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file )
{
    int error_code = 0;
    FILE* include_file = NULL;
    char path[MAX_LINE_SIZE] = {0};
    char *include_file_name;
    int i = 0;

    /* Parce the path to a file */
    strncpy(path, in_file_name, strlen(in_file_name) - 9);

    /* Get the name of the included file */
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
    
    /* If an include file was found, process it */
    if (include_file != NULL ){
        error_code = process_input_file(map,
                include_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);

        fclose(include_file);
    } else {
        error_code = ENOENT;
    }

    return error_code;
}

int handle_true_branch(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    int err_code = 0;
    char new_line[MAX_LINE_SIZE] = {0};

    /* Empty the line */
    strncpy(line, "\0", 1);
    /**
     * Append to the line everything that
     * is not in an else branch
     */

    while (fgets(new_line, MAX_LINE_SIZE, in_file) &&
            strncmp(new_line, "#endif", 6) != 0 ) {
        //printf("%s\n", new_line);
        //print_map(map);
        /* Skip everything that is in an else or elif branch */
        if (strncmp(new_line, "#else", 5) == 0 ||
                strncmp(new_line, "#elif", 5) == 0) {
            while (fgets(new_line, MAX_LINE_SIZE, in_file)
                && strncmp(new_line, "#endif", 6) != 0);
        } else {

            /* Process the line befor adding it to the buffer */
            if (strncmp(new_line, "#", 1) == 0) {
                err_code = process_line(map, new_line,
                        in_file, in_file_name,
                        in_file_dirs, in_file_dir_no,
                        processed_file);
            }
            if (strncmp(new_line, "\n", 1) != 0 &&
                    strncmp(new_line, "#", 1) != 0){
                replace_defines(map, new_line);
                strncat(line, new_line, MAX_LINE_SIZE);
            }
        }
    }
    return err_code;
}

int handle_false_branch(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    int err_code = 0;
    char new_line[MAX_LINE_SIZE] = {0};
    char value[MAX_LINE_SIZE] = {0};

    /**
     *  Skip over lines as long as no endif is found
     *  or no else or elseif branch is found.
     */
    while (fgets(line, MAX_LINE_SIZE, in_file)
            && strncmp(line, "#endif", 6) != 0) {

        if(strncmp(line, "#elif", 5) == 0) {
            /* Get the condition  for else if branches */
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
                        /* Process the line befor adding it to the buffer */
                        if (strncmp(new_line, "#", 1) == 0) {
                            err_code = process_line(map, new_line,
                                    in_file, in_file_name,
                                    in_file_dirs, in_file_dir_no,
                                    processed_file);
                        }
                        if (strncmp(new_line, "\n", 1) != 0 &&
                                strncmp(new_line, "#", 1) != 0){
                            replace_defines(map, new_line);
                            strncat(line, new_line, MAX_LINE_SIZE);
                        }
                    }
                }
                break;
            }
        }

        /* If else statement found */
        if(strncmp(line, "#else", 5) == 0) {
            err_code = handle_true_branch(map, line,
                    in_file, in_file_name,
                    in_file_dirs, in_file_dir_no,
                    processed_file);
            break;
        }
    }

    return err_code;
}

int handle_if_directive(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    int err_code = 0;
    char value[MAX_LINE_SIZE] = {0};

    /* Get the if condition */
    strtok(line, " ");
    strncpy(value, strtok(NULL, "\n"), MAX_LINE_SIZE);

    /* Evaluate the if condifion */
    if(get(map, value) != NULL) {
        replace_defines(map, value);
        if (strcmp(value, "") == 0) {
            strncpy(value, "1\0", 2);
        }
    }

    if (atoi(value) != 0) {
        /* If condition is met */
        err_code = handle_true_branch(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    } else {
        /* If condition not met */
        err_code = handle_false_branch(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    }

    return err_code;
}

int handle_ifdef_directive(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    int err_code = 0;
    char value[MAX_LINE_SIZE] = {0};

    /* Get the condition */
    strtok(line, " ");
    strncpy(value, strtok(NULL, "\n"), MAX_LINE_SIZE);

    /* Evaluate the condition */
    if(get(map, value) != NULL) {
        /* If condition is met */
        err_code = handle_true_branch(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    } else {
        /* If condition not met */
        err_code = handle_false_branch(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    }

    return err_code;
}

int handle_ifndef_directive(hash_map* map, char* line,
        FILE* in_file, char* in_file_name,
        char** in_file_dirs, int in_file_dir_no,
        char* processed_file)
{
    int err_code = 0;
    char value[MAX_LINE_SIZE] = {0};

    /* Get the condition */
    strtok(line, " ");
    strncpy(value, strtok(NULL, "\n"), MAX_LINE_SIZE);

    /* Evaluate the condition */
    if(get(map, value) == NULL) {
        /* If condition is met */
        err_code = handle_true_branch(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    } else {
        /* If condition not met */
        err_code = handle_false_branch(map, line,
                in_file, in_file_name,
                in_file_dirs, in_file_dir_no,
                processed_file);
    }

    return err_code;
}