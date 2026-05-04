#include <math.h>

/*
Vec3 vec3_normalize(Vec3 vec){
    float len2 = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;

    __m128 x = _mm_set_ss(len2);
    __m128 r = _mm_rsqrt_ss(x);

    //Newton: r * (1,5 - 0.5 * len2 * r*r)
    //asm   : r * (3,0 - 0.5 *    x * r*r)
    const __m128 half = _mm_set_ss(0.5f);
    const __m128 three = _mm_set_ss(3.0f);

    __m128 r2     = _mm_mul_ss(r      ,    r);
    __m128 h_len2 = _mm_mul_ss(half   ,    x);
    __m128 term   = _mm_mul_ss(h_len2 ,   r2);
    __m128 nr     = _mm_mul_ss(r      ,mm_sub_ss(three, term));

    float inv_len = _mm_cvtss_f32(nr);

    return (Vec3) {
        .x = inv_len * vec.x,
        .y = inv_len * vec.y,
        .z = inv_len * vec.z
    };
}
*/

//asm   : r * (3,0 - 0.5 *    x * r*r)
Vec2 vec2_normalize(Vec2 vec){
	float len2 = vec.x*vec.x + vec.y*vec.y;
        float32x2_t x = vdup_n_f32(len2);
        float32x2_t r = vrsqrte_f32(x);

	const float32x2 half = vdup_n_f32(0.5f);
	const float32x2 three = vdup_n_f32(3.0f);

	float32x2_t r2 = vmul_f32(r, r);
	float32x2_t h_len2 = vmul_f32(half, x);
	float32x2_t term = vmul_f32(h_len2, r2);
	float32x2_t nr = vmul_f32(r, vsub_f32(three, term));
	float inv = vget_lane_f32(nr, 0);
        return (Vec2){
		vec.x*inv,
		vec.y*inv
	};
}
