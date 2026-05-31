#pragma once

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "SjenaDeskEngine/Math/Vector.h"

typedef struct Color{
    unsigned char r, g, b, a;
}Color;

typedef struct Display{
	void* data;
	Vec2 resolution;
	Vec2 position;
	unsigned int data_size;
	unsigned int properties;
}Display;

void setup_grid(Vec2* resolution);
unsigned int pixel_num(Vec2 resolution);
void render_frame(Display* display);
unsigned char draw_pixel(Display* display, Vec2 dot, Color dot_color);
unsigned char draw_line(Display* display, Vec2 a, Vec2 b, Color color);