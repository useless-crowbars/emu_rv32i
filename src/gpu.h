#ifndef GPU_H
#define GPU_H

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 100

#define SCALE 18
#define GPU_SCREEN_WIDTH (SCREEN_WIDTH * SCALE)
#define GPU_SCREEN_HEIGHT (SCREEN_HEIGHT * SCALE)

#define KB_ESC 0x80
#define KB_SPACE 0x40
#define KB_D 0x20
#define KB_S 0x10
#define KB_A 0x08
#define KB_W 0x04
#define KB_SHIFT 0x02
#define KB_CTRL 0x01

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void init_window(void);
void init_screen(void);
void write_screen(int x, int y, uint32_t color);
void update_screen(void);
void poll_keyboard(void);
void cleanup(void);

extern uint8_t key_pressed;

#endif
