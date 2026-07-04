#pragma once

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "SjenaDeskEngine/Math/Vector.h"

#define ACTIVE               1<<0
#define VISIBLE              1<<1
#define BACKGROUND           1<<2
#define USE_PHYSIC           1<<3
#define USE_TRANSPARENCY     1<<4
#define RESOLUTION_AVAILABLE 1<<5
#define POSITION_AVAILABLE   1<<6
#define DIM_AVAILABLE        1<<7

#define BLACK_LISTED_FLAGS (RESOLUTION_AVAILABLE | POSITION_AVAILABLE | DIM_AVAILABLE)

void set_flag(unsigned char* s_flag, unsigned char t_flag, unsigned char value);

typedef struct GameObject{
	unsigned long long data_size;
	void* data;
}GameObject;

typedef struct GameObjectMgr{
	GameObject* data;
	size_t length;
}GameObjectMgr;

unsigned char game_object_mgr_create(GameObjectMgr* mgr, size_t length);
unsigned char game_object_mgr_destroy(GameObjectMgr* mgr);
unsigned char game_object_create(GameObject* gameObject, uVec2* resolution, uVec2* position, unsigned char dim, unsigned char flag, void* data);
unsigned char game_object_destroy(GameObject* gameObject);
uVec2* game_object_get_resolution(GameObject* gameObject);
uVec2* game_object_get_position(GameObject* gameObject);
unsigned char* game_object_get_dim(GameObject* gameObject);
unsigned char* game_object_get_data(GameObject* gameObject);

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
unsigned char display_free(Display* display);
unsigned char render_frame(Display* display);

unsigned int move_cursor(uVec2 res, uVec2 dot);
unsigned char draw_pixel(Display* display, uVec2 dot, Color dot_color);
unsigned char draw_line(Display* display, uVec2 a, uVec2 b, Color color);
unsigned char display_draw_game_object(Display* display, GameObject* gameObject);
