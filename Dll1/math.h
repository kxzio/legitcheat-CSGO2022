#pragma once
#include "vector3.h"
#include "vector2.h"

#include <cmath>
#include <xmmintrin.h>
#include <pmmintrin.h>
#define M_RADPI 57.295779513082f
#define M_PI 3.14159265358979323846f

static const float invtwopi = 0.1591549f;
static const float twopi = 6.283185f;
static const float threehalfpi = 4.7123889f;
static const float pi = 3.141593f;
static const float halfpi = 1.570796f;
static const __m128 signmask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

static const __declspec(align(16)) float null[4] = { 0.f, 0.f, 0.f, 0.f };
static const __declspec(align(16)) float _pi2[4] = { 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f, 1.5707963267948966192f };
static const __declspec(align(16)) float _pi[4] = { 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f, 3.141592653589793238f };

typedef __declspec(align(16)) union
{
	float f[4];
	__m128 v;
} m128;

__forceinline __m128 sqrt_ps(const __m128 squared) //-V801
{
	return _mm_sqrt_ps(squared);
}

__forceinline __m128 cos_52s_ps(const __m128 x) //-V801
{
	const auto c1 = _mm_set1_ps(0.9999932946f);
	const auto c2 = _mm_set1_ps(-0.4999124376f);
	const auto c3 = _mm_set1_ps(0.0414877472f);
	const auto c4 = _mm_set1_ps(-0.0012712095f);
	const auto x2 = _mm_mul_ps(x, x);
	return _mm_add_ps(c1, _mm_mul_ps(x2, _mm_add_ps(c2, _mm_mul_ps(x2, _mm_add_ps(c3, _mm_mul_ps(c4, x2))))));
}

__forceinline __m128 cos_ps(__m128 angle)
{
	angle = _mm_andnot_ps(signmask, angle);
	angle = _mm_sub_ps(angle, _mm_mul_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_mul_ps(angle, _mm_set1_ps(invtwopi)))), _mm_set1_ps(twopi)));

	auto cosangle = angle;
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(pi), angle))));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(pi)), signmask));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(threehalfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(twopi), angle))));

	auto result = cos_52s_ps(cosangle);
	result = _mm_xor_ps(result, _mm_and_ps(_mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_cmplt_ps(angle, _mm_set1_ps(threehalfpi))), signmask));
	return result;
}

__forceinline __m128 sin_ps(const __m128 angle) //-V801
{
	return cos_ps(_mm_sub_ps(_mm_set1_ps(halfpi), angle));
}

__forceinline void sincos_ps(__m128 angle, __m128* sin, __m128* cos) {
	const auto anglesign = _mm_or_ps(_mm_set1_ps(1.f), _mm_and_ps(signmask, angle));
	angle = _mm_andnot_ps(signmask, angle);
	angle = _mm_sub_ps(angle, _mm_mul_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_mul_ps(angle, _mm_set1_ps(invtwopi)))), _mm_set1_ps(twopi)));

	auto cosangle = angle;
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(pi), angle))));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(pi)), signmask));
	cosangle = _mm_xor_ps(cosangle, _mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(threehalfpi)), _mm_xor_ps(cosangle, _mm_sub_ps(_mm_set1_ps(twopi), angle))));

	auto result = cos_52s_ps(cosangle);
	result = _mm_xor_ps(result, _mm_and_ps(_mm_and_ps(_mm_cmpge_ps(angle, _mm_set1_ps(halfpi)), _mm_cmplt_ps(angle, _mm_set1_ps(threehalfpi))), signmask));
	*cos = result;

	const auto sinmultiplier = _mm_mul_ps(anglesign, _mm_or_ps(_mm_set1_ps(1.f), _mm_and_ps(_mm_cmpgt_ps(angle, _mm_set1_ps(pi)), signmask)));
	*sin = _mm_mul_ps(sinmultiplier, sqrt_ps(_mm_sub_ps(_mm_set1_ps(1.f), _mm_mul_ps(result, result))));
}

namespace math {
	void normalize_angles(vec3& angles);
	float angle_distance(float firstangle, float secondangle);
	void normalize_angle(vec3& angles);
	void normalize_vector(vec3& vec3);
	float normalize_yaw(float f);
	float normalize_pitch(float pitch);
	float vector_normalize(vec3& v);
	void clamp_angles(vec3& angles);
	void vector_transform(const vec3& in1, const matrix& in2, vec3& out);
	vec3 calculate_angle(const vec3& src, const vec3& dst);
	float get_fov(const vec3& viewangles, const vec3& aim_angle);
	void angle_vectors(const vec3& angles, vec3& forward);
	void angle_vectors(const vec3& angles, vec3* forward, vec3* right, vec3* up);
	void vector_angles(const vec3& forward, vec3& angles);
	void vector_angles(const vec3& forward, vec3& up, vec3& angles);
	void VectorMAInline(const float* start, float scale, const float* direction, float* dest);
	void VectorMAInline(const vec3& start, float scale, const vec3& direction, vec3& dest);
	void VectorMA(const vec3& start, float scale, const vec3& direction, vec3& dest);
	void VectorMA(const float* start, float scale, const float* direction, float* dest);
	vec3 cross_product(const vec3& a, const vec3& b);
	float dot_product(const vec3& a, const vec3& b);
	bool world_to_screen(const vec3& origin, vec3& screen, bool direct = false);
	void SmoothAngle(vec3& From, vec3& To, float Percent);
	void rotate_triangle(std::array<vec2, 3>& points, float rotation);
	float random_float(float min, float max);
	int random_int(int min, int max);
	void random_seed(int seed);
	float segment_to_segment(const vec3& s1, const vec3& s2, const vec3& k1, const vec3& k2);
	bool intersect_line_with_bb(vec3& start, vec3& end, vec3& min, vec3& max);
	vec3 vector_rotate(const vec3& in1, const matrix& in2);
	vec3 vector_rotate(const vec3& in1, const vec3& in2);
	matrix angle_matrix(const vec3& angles);
	void angle_matrix(const vec3& ang, const vec3& pos, matrix& out);
	void vector_rotate(const vec3& in1, const matrix& in2, vec3& out);
	vec3 matrix_get_origin(const matrix& src);
	void matrix_set_origin(const vec3& pos, matrix& matrix);
	void fast_rsqrt(float a, float* out);
	float fast_vec_normalize(vec3& vec);
	void matrix_copy(const matrix& in, matrix& out);
	void concat_transforms(const matrix& in1, const matrix& in2, matrix& out);
	double fast_sin(double x);
	float simple_spline(float value);
	float simple_spline_remap_val_clamped(float value, float a, float b, float c, float d);
	float direction_difference(const vec3& first, const vec3& second);

	template<class T, class U>
	static T clamp(const T& in, const U& low, const U& high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	template <typename t>
	static t interpolate(const t& t1, const t& t2, float progress)
	{
		if (t1 == t2)
			return t1;

		return t2 * progress + t1 * (1.0f - progress);
	}

	template <typename t>
	static t lerp(const t& t1, const t& t2, float progress)
	{
		return t1 + (t2 - t1) * progress;
	}

	template <typename t>
	static t lerp2(float progress, const t& t1, const t& t2)
	{
		return t1 + (t2 - t1) * progress;
	}
}