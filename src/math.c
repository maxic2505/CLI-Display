#include <math.h>

//asm   : r * (3,0 - 0.5 *    x * r*r)
Vec2f vec2f_normalize(Vec2 vec){
	float len2 = vec.x*vec.x + vec.y*vec.y;
        float32x2_t x = vdup_n_f32(len2);
        float32x2_t r = vrsqrte_f32(x);

	const float32x2_t half = vdup_n_f32(0.5f);
	const float32x2_t third = vdup_n_f32(1.5f);

	float32x2_t r2 = vmul_f32(r, r);
	float32x2_t h_len2 = vmul_f32(half, x);
	float32x2_t term = vmul_f32(h_len2, r2);
	float32x2_t nr = vmul_f32(r, vsub_f32(third, term));
	float inv = vget_lane_f32(nr, 0);
        return (Vec2f){
		vec.x*inv,
		vec.y*inv
	};
}
