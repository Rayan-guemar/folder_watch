#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include "../include/folder_info.h"

static struct termios orig_termios;


void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void show_cursor() {
    printf("\e[?25h");
    fflush(stdout);
}

void hide_cursor() {
    printf("\e[?25l");
    fflush(stdout);
}

void clear_screen(void) {
    printf("\033[H\033[J");
}