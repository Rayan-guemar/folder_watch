#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <termios.h>

#include "../include/folder_info.h"
#include "../include/terminal.h"
#include "../include/ui.h"

void on_write(struct folder_info* _f) {
    update(_f);
}

void on_rename(struct folder_info* _f) {
    printf("Directory %s moved or renamed, can't find new location or name\n", _f->name);
    printf("exiting program\n");
    sleep(1);
    exit(EXIT_FAILURE);
}

void on_delete(struct folder_info* _f) {
    printf("Directory %s deleted\n", _f->name);
    printf("exiting program\n");
    sleep(1);
    exit(EXIT_FAILURE);
}

void on_attrib(struct folder_info* _f) {
    _f->has_right_modified = 1;
}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("missing argument (dir name)");
        exit(EXIT_FAILURE);
    }

    char* dir_name = argv[1];

    struct folder_info* f = init(dir_name);

    if (f == NULL) {
        printf("error while opening the folder");
        exit(EXIT_FAILURE);
    }

    int kq = kqueue();

    struct kevent change;
    EV_SET(&change, f->fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_RENAME | NOTE_WRITE | NOTE_ATTRIB | NOTE_DELETE, 0, NULL);
    kevent(kq, &change, 1, NULL, 0, NULL);


    struct kevent triggereds[16];

    atexit(show_cursor);

    enable_raw_mode();


    while (1) {

        display_folder(f);
        clear(f);
        int nev = kevent(kq, NULL, 0, triggereds, 16, NULL);

        if (nev >= 0) {

            for (int i = 0; i < nev; i++) {
                int fflags = triggereds[i].fflags;

                if (fflags & NOTE_WRITE) on_write(f);
                if (fflags & NOTE_RENAME) on_rename(f);
                if (fflags & NOTE_ATTRIB)  on_attrib(f);
                if (fflags & NOTE_DELETE)  on_delete(f);
            }
        }

    }
}