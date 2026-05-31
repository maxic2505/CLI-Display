#include "Render.h"

// Display | Setup | 0 = Success
unsigned char display_init(Display* display){
	// Resolution
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	display->resolution.x = w.ws_col;
	display->resolution.y = w.ws_row;
	// Pixel-Data
	display->data_size = display->resolution.x * display->resolution.y;
	size_t size = display->data_size*4;
	display->data = malloc(size);
	if(!display->data)return 1;
	memset(display->data, 0, size);
	return 0;
}

unsigned char render_frame(Display* display){
	printf("\033c\033[H");
	fflush(stdout);

	char* buffer = malloc(display->data_size*20);
	if(!buffer)return 1;
	unsigned int rendered = 0;
	unsigned int offset = 0;
	Color last_color = {0};

	Color* color_arr = (Color*)display->data;
	for(int i = 0; i<display->data_size; i++){
		Color color = color_arr[i];
		if(color.raw == last_color.raw) offset += sprintf(&buffer[offset], "X");
		else offset += sprintf(&buffer[offset], "\033[38;2;%d;%d;%dmX", color.r, color.g, color.b); rendered++;
		last_color = color;
		if(rendered >= display->resolution.x){
			offset += sprintf(&buffer[offset], "\n");
			rendered = 0;
		}
	}
	fwrite(buffer, 1, offset, stdout);
	fflush(stdout);
	free(buffer);
}

unsigned int move_cursor(uVec2 res, uVec2 dot){
	return res.x*dot.y+dot.x;
}

unsigned char draw_pixel(Display* display, uVec2 dot, Color dot_color){
	if(!display || !display->data)return 1;

	unsigned dot_num = display->resolution.x*dot.y+dot.x;
	if(dot_num > display->data_size) return 1;
	((Color*)display->data)[dot_num] = dot_color;
	return 0;
}

int positive(int var){
	if(var<0)return var * -1;
	else return var;
}

unsigned char draw_line(Display* display, uVec2 a, uVec2 b, Color color){
	if(!display || !display->data)return 1;
	
	Color* color_arr = (Color*)display->data;

	int dx = (int)b.x-(int)a.x;
	int dy = (int)b.y-(int)a.y;

	int px = positive(dx);
	int py = positive(dy);
	int steps = px > py ? px : py;

	float x_inc = dx/(float)steps;
	float y_inc = dy/(float)steps;

	Vec2 brush = (Vec2){.x=(float)a.x, .y=(float)a.y};
	for(int i = 0; i<=steps; i++){
		draw_pixel(display, (uVec2){.x=(unsigned int)(brush.x+0.5f), .y=(unsigned int)(brush.y+0.5f)}, color);
		brush.x += x_inc;
		brush.y += y_inc;
	}

	return 0;
}