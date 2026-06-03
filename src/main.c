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
		printf("ERROR: GAMEOBJECTS MISSING\n");
		display_free(&display);
		return 1;
	}else memset(gameObjects, 0, g_o_size * sizeof(GameObject));

	if(game_object_create(&gameObjects[0], NULL, NULL, 0, ACTIVE | VISIBLE | BACKGROUND, &(Color){.r=75, .g=75, .b=150, .a=255})){
		printf("ERROR: GAMEOBJECT 0\n");
		free(gameObjects);
		display_free(&display);
		return 1;
	}

	display_draw_game_object(&display, &gameObjects[0]);

	for(unsigned char i = 0; i<255; i++){
		render_frame(&display);
		((Color*)(gameObjects[0].data+1))->r = i;;
		display_draw_game_object(&display, &gameObjects[0]);
		usleep(200000);
	}
	
	printf("Resolution: (%d|%d)\nPIXEL_SIZE: %d\n", display.resolution.x, display.resolution.y, display.data_size);

	// SAFE CLEANUP
	free(gameObjects);
	display_free(&display);
	return 0;
}
