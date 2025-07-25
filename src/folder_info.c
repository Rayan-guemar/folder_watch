#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/folder_info.h"
#include "../include/utils.h"

void get_new_childs_name(struct folder_info* _f, char*** _new_names, int* _new_names_length) {
    DIR* dir = opendir(_f->name);

    *_new_names = NULL;
    *_new_names_length = 0;

    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent* entry;

    char** names = NULL;
    int names_size = 0;

    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        fflush(stdout);
        char* name = strdup(entry->d_name);
        add_array((void**)&names, &names_size, &name, sizeof(char*));

    }

    *_new_names = names;
    *_new_names_length = names_size;

    closedir(dir);
}


void add_deleted_file_names(struct folder_info* _f, char* _name) {
    char* name_copy = strdup(_name);
    add_array((void**)&_f->deleted_file_names, &_f->deleted_file_names_length, &name_copy, sizeof(char*));
}

void add_added_files_indexes(struct folder_info* _f, int i) {
    add_array((void**)&_f->added_files_indexes, &_f->added_files_indexes_length, &i, sizeof(int));
}

void add_childs_name(struct folder_info* _f, char* _name) {
    char* name_copy = _name;
    add_array((void**)&_f->childs_name, &_f->childs_name_length, &name_copy, sizeof(char*));
}

void compare_childs_name(struct folder_info* _f, char** _new_childs, int _new_childs_length) {
    char** old_childs = _f->childs_name;

    int i = 0;
    int j = 0;

    sort_by_name(old_childs, _f->childs_name_length);
    sort_by_name(_new_childs, _new_childs_length);

    while (i < _f->childs_name_length || j < _new_childs_length) {

        int comp;
        if (i >= _f->childs_name_length) {
            comp = 1;
        } else if (j >= _new_childs_length) {
            comp = -1;
        } else {
            comp = strcmp(old_childs[i], _new_childs[j]);
        }

        if (comp == 0) {
            i++;
            j++;
        } else if (comp < 0) {
            add_deleted_file_names(_f, old_childs[i]);
            i++;
        } else {
            add_added_files_indexes(_f, j);
            j++;
        }
    }

}

void update(struct folder_info* _f) {
    char** new_names = NULL;
    int new_names_length = 0;


    get_new_childs_name(_f, &new_names, &new_names_length);

    compare_childs_name(_f, new_names, new_names_length);

    for (int i = 0; i < _f->childs_name_length; i++) {
        free(_f->childs_name[i]);
    }
    free(_f->childs_name);

    _f->childs_name = new_names;
    _f->childs_name_length = new_names_length;
}

void clear(struct folder_info* _f) {

    if (_f->added_files_indexes != NULL) {
        free(_f->added_files_indexes);
        _f->added_files_indexes = NULL;
    }
    _f->added_files_indexes_length = 0;

    for (int i = 0; i < _f->deleted_file_names_length; i++) {
        free(_f->deleted_file_names[i]);
    }
    _f->deleted_file_names_length = 0;

    _f->has_right_modified = 0;
    fflush(stdout);

}

struct folder_info* init(char* name) {

    struct folder_info* f = malloc(sizeof(struct folder_info));

    if (!f) {
        return NULL;
    }

    f->childs_name = NULL;
    f->childs_name_length = 0;

    f->added_files_indexes = NULL;
    f->added_files_indexes_length = 0;

    f->deleted_file_names = NULL;
    f->deleted_file_names_length = 0;

    f->name = strdup(name);

    f->fd = open(name, O_RDONLY);

    if (f->fd < 0) {
        perror("open");
        free(f);
        return NULL;
    }
    get_new_childs_name(f, &f->childs_name, &f->childs_name_length);

    return f;
}

void free_folder_info(struct folder_info* _f) {
    if (!_f) return;


    if (_f->fd >= 0) close(_f->fd);

    free(_f->name);


    for (int i = 0; i < _f->childs_name_length; i++) {
        free(_f->childs_name[i]);
    }
    if (_f->childs_name != NULL) free(_f->childs_name);


    for (int i = 0; i < _f->deleted_file_names_length; i++) {
        free(_f->deleted_file_names[i]);
    }
    free(_f->deleted_file_names);


    free(_f->added_files_indexes);

    free(_f);
}



