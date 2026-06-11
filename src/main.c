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
	
	GameObjectMgr mgr = {0};
	if(game_object_mgr_create(&mgr, 5)){
		printf("ERROR: GAMEOBJECTS MISSING\n");
		display_free(&display);
		return 1;
	}

	if(game_object_create(&mgr.data[0], NULL, NULL, 0, ACTIVE | VISIBLE | BACKGROUND, &(Color){.r=75, .g=75, .b=150, .a=255})){
		printf("ERROR: GAMEOBJECT 0\n");
		game_object_mgr_destroy(&mgr);
		display_free(&display);
		return 1;
	}

	display_draw_game_object(&display, &mgr.data[0]);

	uVec2* resolution = game_object_get_resolution(&mgr.data[0]);
	uVec2* position = game_object_get_position(&mgr.data[0]);
	unsigned char* dim = game_object_get_dim(&mgr.data[0]);
	Color* color_ptr = (Color*)game_object_get_data(&mgr.data[0]);

	for(unsigned char i = 0; i<255; i++){
		render_frame(&display);
		*color_ptr = (Color){.r=255-i, .g=i, .b=255-i, .a=0};
		display_draw_game_object(&display, &mgr.data[0]);
		//usleep(10000);
	}

	// DEBUG DATA
	printf("Resolution: (%d|%d)\nPIXEL_SIZE: %d\n", display.resolution.x, display.resolution.y, display.data_size);
	printf("GameObject---------\n");
	if(resolution)printf("res: .x = %u, .y = %u\n", resolution->x, resolution->y);
	if(position)printf("pos: .x = %u, .y = %u\n", position->x, position->y);
	if(dim)printf("dim: %d\n", *dim);

	// SAFE CLEANUP
	game_object_mgr_destroy(&mgr);
	display_free(&display);
	return 0;
}
