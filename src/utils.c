#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void add_array(void** _array, int* _array_length, void* _el, int _el_size) {

    void* tmp = realloc(*_array, _el_size * (*_array_length + 1));

    if (tmp == NULL) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }

    *_array = tmp;

    void* ptn_location = (char*)(*_array) + ((*_array_length) * _el_size);

    memcpy(ptn_location, _el, _el_size);

    (*_array_length)++;
}

int compare(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);
}

void sort_by_name(char** array, int length) {
    qsort(array, length, sizeof(char*), compare);
}