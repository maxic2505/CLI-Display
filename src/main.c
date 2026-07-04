#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Render.h"
#include "SjenaDeskEngine/Input/Input.h"

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
	// BACKGROUND
	if(game_object_create(&mgr.data[0], NULL, NULL, 0, ACTIVE | VISIBLE | BACKGROUND, &(Color){.r=75, .g=75, .b=150, .a=255})){
		printf("ERROR: GAMEOBJECT 0\n");
		game_object_mgr_destroy(&mgr);
		display_free(&display);
		return 1;
	}
	display_draw_game_object(&display, &mgr.data[0]);
	render_frame(&display);

	// *PLAYER
	Color data[9] = {0};
	for(int i = 0; i<9; i++){
		data[i] = (Color){.r = 255, .g = 0, .b = 255, .a = 255};
	}
	if(game_object_create(&mgr.data[1], &(uVec2){.x = 3, .y = 3}, &(uVec2){.x = 0, .y = 0}, 0, ACTIVE | VISIBLE, data)){
		printf("ERROR: GAMEOBJECT 1\n");
		game_object_mgr_destroy(&mgr);
		display_free(&display);
		return 1;
	}
	display_draw_game_object(&display, &mgr.data[1]);

	//uVec2* resolution = game_object_get_resolution(&mgr.data[0]);
	uVec2* position = game_object_get_position(&mgr.data[1]);
	//unsigned char* dim = game_object_get_dim(&mgr.data[0]);
	//Color* color_ptr = (Color*)game_object_get_data(&mgr.data[0]);
	input_key_setup_api();
	unsigned char running = 1;
	while(running){
		input_key_handler_api();
		if(getKey(KEYCODE_ESC))running = 0;
		if(getKeyDown(KEYCODE_W))position->y--;
		if(getKeyDown(KEYCODE_A))position->x--;
		if(getKeyDown(KEYCODE_S))position->y++;
		if(getKeyDown(KEYCODE_D))position->x++;
		display_draw_game_objects_mgr(&display, &mgr);
		render_frame(&display);
		usleep(33000);
	}
	
	// SAFE CLEANUP
	game_object_mgr_destroy(&mgr);
	display_free(&display);
	return 0;
}
