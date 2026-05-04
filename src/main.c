#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct Display{
	void* data;
	Vec2 resolution;
	Vec2 position;
	unsigned int data_size;
	unsigned int properties;
}Display;

void setup_grid(Vec2* resolution){
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	resolution->x = w.ws_col;
	resolution->y = w.ws_row;
}

unsigned int pixel_num(Vec2 resolution){
	return resolution.x * resolution.y;
}

void render_frame(Display* display){
	printf("\033c\033[H");
	fflush(stdout);

	unsigned int rendered = 0;

	for(int i = 0; i<display->data_size; i++){
		printf("\033[38;2;%d;%d;%dmX\033[0m", ((Color*)display->data)[i].r, ((Color*)display->data)[i].g, ((Color*)display->data)[i].b);rendered++;
		if(rendered >= display->resolution.x){
			printf("%c", (i == display->data_size) ? '\n' : '\0');
			rendered = 0;
		}
	}
	fflush(stdout);
}

unsigned char draw_pixel(Display* display, Vec2 dot, Color dot_color){
	if(!display || !display->data)return 1;

	unsigned dot_num = display->resolution.x*dot.y+dot.x;
	if(dot_num > display->data_size) return 1;
	((Color*)display->data)[dot_num] = dot_color;
	return 0;
}
unsigned char draw_line(Display* display, Vec2 a, Vec2 b, Color color){
	if(!display || !display->data)return 1;
	return 0;
}


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
