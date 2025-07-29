#ifndef FOLDER_INFO_H
#define FOLDER_INFO_H

struct folder_info {
    char* name;
    int fd;
    int has_right_modified;
    int has_renamed;
    int has_deleted;

    char** childs_name;
    int childs_name_length;

    int* added_files_indexes;
    int added_files_indexes_length;

    char** deleted_file_names;
    int deleted_file_names_length;
};


void update(struct folder_info* _f);

void clear(struct folder_info* _f);

struct folder_info* init(char* name);

void free_folder_info(struct folder_info* _f);


#endif


