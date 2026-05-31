#pragma once

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "SjenaDeskEngine/Math/Vector.h"

typedef union{
	unsigned int raw;
	struct{
		unsigned char a;
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};
}Color;

typedef struct Display{
	void* data;
	uVec2 resolution;
	uVec2 position;
	unsigned int data_size;
	unsigned int properties;
}Display;

unsigned char display_init(Display* display);
unsigned char render_frame(Display* display);
unsigned char draw_pixel(Display* display, uVec2 dot, Color dot_color);
unsigned char draw_line(Display* display, uVec2 a, uVec2 b, Color color);