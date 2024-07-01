#ifndef GPU_H
#define GPU_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void init_screen();
void write_screen(uint32_t x, uint32_t y, uint32_t color);
void update_screen();
void poll_keyboard();
void cleanup();

extern bool* end;
extern char* key_changed;
extern bool* key_pressed;
extern bool* change_ack;

enum KEYS {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE,
	CTRL,
};

#endif
