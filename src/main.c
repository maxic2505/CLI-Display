#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Render.h"




int main(){
	Display display = {0};

	if(display_init(&display)){
		printf("ERROR: DISPLAY INIT FAILED\n");
		return 1;
	}
	
	size_t g_o_size = 5;
	GameObject* gameObjects = {0};

	gameObjects = malloc(g_o_size * sizeof(GameObject));
	if(!gameObjects){
		printf("ERROR: GAMEOBJECTS MISSING\n")
		display_free(&display);
		return 1;
	}else memset(gameObjects, 0, g_o_size * sizeof(GameObject));

	

	draw_line(&display, (uVec2){.x=display.resolution.x-1, .y=0}, (uVec2){.x=0, .y=display.resolution.y-1}, (Color){.raw=0xFF000000});
	draw_line(&display, (uVec2){.raw = 0}, (uVec2){.x = display.resolution.x-1, .y = display.resolution.y-1}, (Color){.raw = 0x00FF0000});
	render_frame(&display);
	sleep(2);

	printf("Resolution: (%d|%d)\nPIXEL_SIZE: %d\n", display.resolution.x, display.resolution.y, display.data_size);

	return 0;
}
