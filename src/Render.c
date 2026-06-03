#include "Render.h"

void set_flag(unsigned char* s_flag, unsigned char t_flag, unsigned char value){
	if(!s_flag)return;
	if(value) *s_flag |= t_flag;
	else *s_flag &= ~t_flag;
}

unsigned char game_object_mgr_create(GameObjectMgr* mgr, size_t length){
	if(!mgr || mgr->data || length < 1)return 1;
	mgr->data = malloc(length * sizeof(GameObject));
	if(!mgr->data)return 1;
	mgr->length = length;
	return 0;
}
unsigned char game_object_mgr_destroy(GameObjectMgr* mgr){
	if(!mgr || !mgr->data)return 1;
	for(int i = 0; i<(mgr->length); i++){
		if(mgr->data[i].data)free(mgr->data[i].data);
	}
	free(mgr->data);
	mgr->data = NULL;
	mgr->length = 0;
	return 0;	
}

unsigned char game_object_create(GameObject* gameObject, uVec2* resolution, uVec2* position, unsigned char dim, unsigned char flag, void* data){
	if(!gameObject || !data) return 1;

	if(flag & BLACK_LISTED_FLAGS) return 1;
	
	unsigned char sys_flag = 0;
	size_t data_size = 1;

	if(resolution) {
		set_flag(&sys_flag, RESOLUTION_AVAILABLE, 1);
		data_size += resolution->x*resolution->y;
		data_size += sizeof(uVec2);
	}else data_size += sizeof(Color);
	if(position){
		set_flag(&sys_flag, POSITION_AVAILABLE, 1);
		data_size += (flag & USE_PHYSIC) ? sizeof(uVec2*) : sizeof(uVec2);
	}
	if(dim) {
		set_flag(&sys_flag, DIM_AVAILABLE, 1);
		data_size++;
	}

	unsigned char* sys_data = malloc(data_size);
	if(!sys_data) return 1;

	size_t offset = 0;

	((unsigned char*)sys_data)[offset++] = sys_flag | flag;
	
	if(resolution){
		*((uVec2*)(sys_data+offset)) = *resolution;
		offset+=sizeof(uVec2);
	}
	if(position){
		if(flag & USE_PHYSIC) *((uVec2**)(sys_data+offset)) = position;
		else *((uVec2*)(sys_data+offset)) = *position;
		offset+=(flag & USE_PHYSIC) ? sizeof(uVec2*) : sizeof(uVec2);
	}

	Color* color_d = (Color*)(sys_data+offset);
	for(int i = 0; i < ((resolution) ? (resolution->x*resolution->y) : 1); i++){
		color_d[i] = ((Color*)data)[i];
	}
	
	gameObject->data = (void*)sys_data;
	gameObject->data_size = data_size;
	return 0;
}

unsigned char game_object_destroy(GameObject* gameObject){
	if(!gameObject || !gameObject->data)return 1;
	free(gameObject->data);
	gameObject->data = NULL;
	gameObject->data_size = 0;
	return 0;
}

// Display | Setup | 0 = Success
unsigned char display_init(Display* display){
	// Resolution
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	display->resolution.x = w.ws_col;
	display->resolution.y = w.ws_row;
	// Pixel-Data
	display->data_size = display->resolution.x * display->resolution.y;
	size_t size = display->data_size*sizeof(Color);
	display->data = malloc(size);
	if(!display->data)return 1;
	memset(display->data, 0, size);
	return 0;
}
// Display | Setup | 0 = Success
unsigned char display_free(Display* display){
	if(!display)return 1;
	if(display->data){
		free(display->data);
		display->data = NULL;
	}
	memset(display, 0, sizeof(Display));
	return 0;
}

unsigned char display_draw_game_object(Display* display, GameObject* gameObject){
	if(!display || !display->data || !gameObject || !gameObject->data) return 1;

	Color* pixel = (Color*)display->data;

	unsigned char* data_ptr = (unsigned char*)gameObject->data;
	unsigned char data_flags = data_ptr[0];
	uVec2 resolution = {0};
	uVec2 local_position = {0};
	uVec2* phys_position = NULL;

	unsigned int pixel_num = (display->resolution.x * display->resolution.y);

	size_t offset = 1; // 1 because data[0] = properties;
	
	if(data_flags | BACKGROUND){
		Color pixel_data = *(Color*)(data_ptr+offset);
		for(int i = 0; i<pixel_num; i++){
			((Color*)(display->data))[i] = pixel_data;
		}
	}else{
		if(data_flags & RESOLUTION_AVAILABLE){
			uVec2 resolution = *((uVec2*)(data_ptr+offset));
			offset += sizeof(uVec2);
		}
		if(data_flags & POSITION_AVAILABLE){
			if(data_flags & USE_PHYSIC) phys_position = *((uVec2**)(data_ptr+offset)); 
			else local_position = *((uVec2*)(data_ptr+offset));
			offset += ((data_flags & USE_PHYSIC) ? sizeof(uVec2*) : sizeof(uVec2));
		}
		Color* pixel_data =	(Color*)(data_ptr+offset);
		for(int i = 0; i<pixel_num; i++){
			((Color*)(display->data))[i] = pixel_data[i];
		}
	}
	
	return 0;
}

unsigned char render_frame(Display* display){
	printf("\033[H");
	fflush(stdout);

	char* buffer = malloc((display->data_size*20) + display->resolution.y + 1);
	if(!buffer)return 1;
	unsigned int rendered = 0;
	unsigned int rendered_y = 0;
	unsigned int offset = 0;
	Color last_color = {0};

	unsigned char first_pixel = 1;

	Color* color_arr = (Color*)display->data;
	for(int i = 0; i<display->data_size; i++){
		Color color = color_arr[i];
		if(!first_pixel && color.raw == last_color.raw) buffer[offset++] = 'X';
		else {
			offset += sprintf(&buffer[offset], "\033[38;2;%d;%d;%dmX", color.r, color.g, color.b);
			first_pixel = 0;
		}
		rendered++;
		last_color = color;
		if(rendered >= display->resolution.x){
			rendered_y++;
			if(rendered_y < display->resolution.y)buffer[offset++] = '\n';
			rendered = 0;
		}
	}
	fwrite(buffer, 1, offset, stdout);
	fflush(stdout);
	free(buffer);
	return 0;
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
