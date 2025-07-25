#ifndef TERMINAL
#define TERMINAL

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include "../include/folder_info.h"

// CHAT-GPT
// TODO: Comprendre comment ça marche
static struct termios orig_termios;


// CHAT-GPT
// TODO: Comprendre comment ça marche
void disable_raw_mode();

// CHAT-GPT
// TODO: Comprendre comment ça marche
void enable_raw_mode();

void show_cursor();

void hide_cursor();

void clear_screen(void);

#endif