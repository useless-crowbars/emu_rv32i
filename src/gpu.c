#include "gpu.h"
#include "mem.h"

bool end = false;
char key_changed;
bool key_pressed;
bool change_ack = true;

#ifdef __riscv
#else

#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;
uint32_t *screen;
void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods);

void init_window()
{
	if (!glfwInit()) {
		exit(1);
	}

	window = glfwCreateWindow(GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, "emu_rv32im", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
}

void init_screen()
{
	init_window();

	screen = (uint32_t *)malloc(GPU_SCREEN_WIDTH * GPU_SCREEN_HEIGHT * sizeof(uint32_t));
	if (!screen) {
		exit(1);
	}

	for (int y = 0; y < GPU_SCREEN_HEIGHT; y++) {
		for (int x = 0; x < GPU_SCREEN_WIDTH; x++) {
			screen[y * GPU_SCREEN_WIDTH + x] = 0xffff0000;
		}
	}
}

void write_screen(int x, int y, uint32_t color)
{
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return;
	}

	for (int j = 0; j < SCALE; j++) {
		for (int i = 0; i < SCALE; i++) {
			screen[(y * SCALE + j) * GPU_SCREEN_WIDTH + x * SCALE + i] = color;
		}
	}
}

void update_screen()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screen);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void poll_keyboard()
{
	glfwPollEvents();
}

void cleanup()
{
	glfwTerminate();
	free(screen);
}

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		end = true;
	}

	if (!change_ack) {
		return;
	}

	switch (key) {
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		key_changed = LEFT;
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		key_changed = RIGHT;
		break;
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		key_changed = UP;
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		key_changed = DOWN;
		break;
	case GLFW_KEY_SPACE:
		key_changed = SPACE;
		break;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:
		key_changed = CTRL;
		break;
	default:
		break;
	}

	key_pressed = (action == GLFW_PRESS);
	change_ack = false;
}

#endif
