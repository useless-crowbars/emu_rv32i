#include "gpu.h"
#include "mem.h"
#include <stdint.h>

uint8_t key_pressed = 0;

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
		printf("error write_screen: %d %d %x\n", x, y, color);
		exit(1);
		return;
	}

	for (int j = 0; j < SCALE; j++) {
		for (int i = 0; i < SCALE; i++) {
			screen[((SCREEN_HEIGHT - 1 - y) * SCALE + j) * GPU_SCREEN_WIDTH + x * SCALE + i] = color;
		}
	}
}

uint8_t status= 0;

void update_screen()
{

	uint8_t gpu_begin = get_b(0x10031fff);
	if(status != gpu_begin) {
		status = gpu_begin;
		for(int i = 0; i < SCREEN_HEIGHT; i++) {
			for(int j = 0; j < SCREEN_WIDTH; j++) {
				uint32_t color = 0xff1f1f1f;

				uint8_t val = *((uint8_t*)mem[GPU] + gpu_begin* 0x4000 + i* SCREEN_WIDTH + j);
				color |= (val & 0x07) << 5;
				color |= (val & 0x38) << (13 - 3);
				color |= (val & 0xc0) << (22 - 6);

				write_screen(j, i, color);
			}
		}
	}
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
	switch (key) {
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			{
				if (action == GLFW_PRESS) {
					key_pressed |= KB_A;
				} else if (action == GLFW_RELEASE) {
					key_pressed &= (uint8_t)~KB_A;
				}
			}
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			{
				if (action == GLFW_PRESS) {
					key_pressed |= KB_D;
				} else if (action == GLFW_RELEASE) {
					key_pressed &= (uint8_t)~KB_D;
				}
			}
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			{
				if (action == GLFW_PRESS) {
					key_pressed |= KB_W;
				} else if (action == GLFW_RELEASE) {
					key_pressed &= (uint8_t)~KB_W;
				}
			}
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			{
				if (action == GLFW_PRESS) {
					key_pressed |= KB_S;
				} else if (action == GLFW_RELEASE) {
					key_pressed &= (uint8_t)~KB_S;
				}
			}
			break;
		case GLFW_KEY_SPACE:
			{
				if (action == GLFW_PRESS) {
					key_pressed |= KB_SPACE;
				} else if (action == GLFW_RELEASE) {
					key_pressed &= (uint8_t)~KB_SPACE;
				}
			}
			break;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL:
			{
				if (action == GLFW_PRESS) {
					key_pressed |= KB_CTRL;
				} else if (action == GLFW_RELEASE) {
					key_pressed &= (uint8_t)~KB_CTRL;
				}
			}
			break;
		default:
			break;
	}
}

#endif
