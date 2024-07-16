#ifndef GPU_H
#define GPU_H

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 100

#define SCALE 12
#define GPU_SCREEN_WIDTH (SCREEN_WIDTH * SCALE)
#define GPU_SCREEN_HEIGHT (SCREEN_HEIGHT * SCALE)

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void init_window(void);
void init_screen(void);
void write_screen(int x, int y, uint32_t color);
void update_screen(void);
void poll_keyboard(void);
void cleanup(void);

extern bool end;
extern char key_changed;
extern bool key_pressed;
extern bool change_ack;

enum KEYS {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE,
	CTRL,
};

#endif
