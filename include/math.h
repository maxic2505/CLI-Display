#pragma once

#include <arm_neon.h>

typedef struct Vec2{
        unsigned int x, y;
}Vec2;

typedef struct Vec2f{
	float x, y;
}Vec2f;

Vec2f vec2f_normalize(Vec2 vec);

typedef struct Color{
        unsigned char r, g, b, a;
}Color;
