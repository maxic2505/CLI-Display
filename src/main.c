#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Render.h"




int main(){
	Display display = {0};

	setup_grid(&display.resolution);
	display.data_size = pixel_num(display.resolution);
	display.data = malloc(sizeof(Color[display.data_size]));
	if(display.data == NULL)return printf("ERROR: MALLOC FRAME DATA\n");
	memset(display.data, 0x00, sizeof(Color[display.data_size]));

	draw_pixel(&display, (Vec2){0,0}, (Color){255,255,255,0});
	draw_pixel(&display, (Vec2){display.resolution.x-1,0},  (Color){255,255,255,0});
	draw_pixel(&display, (Vec2){0, display.resolution.y-1}, (Color){255,255,255,0});
	draw_pixel(&display, (Vec2){display.resolution.x-1, display.resolution.y-1}, (Color){255,255,255,0});

	render_frame(&display);
	sleep(2);

	printf("Resolution: (%d|%d)\nPIXEL_SIZE: %d\n", display.resolution.x, display.resolution.y, display.data_size);

	Vec2f test = vec2f_normalize(display.resolution);

	printf("Normalized Res: %2.2f|%2.2f", test.x, test.y);

	return 0;
}
