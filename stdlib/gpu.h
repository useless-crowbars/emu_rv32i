#ifndef GPU_H
#define GPU_H

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 100

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void init_screen();
void write_screen(uint32_t x, uint32_t y, uint32_t color);
void update_screen();
void poll_keyboard();
void cleanup();

#endif
