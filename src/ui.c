#include <stdio.h>
#include <string.h> 


#include "../include/folder_info.h"
#include "../include/terminal.h"
#include "../include/ui.h"

char* generate_line(char symbol, int n) {
    char* line = malloc(sizeof(char) * (n + 1));
    for (int i = 0; i < n; i++) {
        line[i] = symbol;
    }
    line[n] = '\0';
    return line;
}

char* generate_bottom_line(char* name) {
    return generate_line('-', EMPTY_BAR_LENGTH + strlen(name));
}

char* generate_top_line(char* name) {

    int total_len = EMPTY_BAR_LENGTH + strlen(name);
    char* line = malloc(sizeof(char) * (total_len + 1));

    if (!line) {
        return NULL;
    }

    char* semi_line1 = generate_line('-', EMPTY_BAR_LENGTH / 2);
    char* semi_line2 = generate_line('-', EMPTY_BAR_LENGTH / 2);


    snprintf(line, total_len + 1, "%s%s%s", semi_line1, name, semi_line2);

    free(semi_line1);
    free(semi_line2);

    return line;
}





void display_folder(struct folder_info* _f) {

    system("clear");
    printf("\e[?25l");
    fflush(stdout);
    printf("\33[2K\r");

    char* folder_name;

    if (_f->has_renamed) {
        folder_name = "?";
    } else {
        folder_name = _f->name;
    }
    printf("%s\n\n", generate_top_line(folder_name));

    for (int i = 0; i < _f->childs_name_length; i++) {
        char* name = _f->childs_name[i];
        int is_added = 0;

        for (int j = 0; j < _f->added_files_indexes_length; j++) {

            if (i == _f->added_files_indexes[j]) {
                is_added = 1;
                break;
            }
        }

        if (is_added) {
            printf("\033[32m  - %s (+)\033[0m\n", name);
        } else {
            printf("  - %s\n", name);
        }
    }

    printf("\n%s", generate_bottom_line(folder_name));

    if (_f->deleted_file_names_length > 0 && _f->deleted_file_names != NULL) {
        if (_f->deleted_file_names_length > 0) {
            printf("\033[31m Deletion: ");
        }
        for (int i = 0; i < _f->deleted_file_names_length; i++) {
            printf("%s, ", _f->deleted_file_names[i]);
        }
        if (_f->deleted_file_names_length > 0) {
            printf("\033[0m\n");
        }
    }



    if (_f->has_right_modified == 1) {
        printf("Directory rights have been modified\n");
    }
}

