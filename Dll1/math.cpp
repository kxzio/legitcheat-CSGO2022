#include "math.h"
#include "interfaces.h"
#include "globals.h"
#include <Windows.h>
#include <Psapi.h>
#include <array>

namespace math
{
	void normalize_angles(vec3& angles)
	{
		while (angles.x > 89.0f)
			angles.x -= 180.0f;

		while (angles.x < -89.0f)
			angles.x += 180.0f;

		while (angles.y < -180.0f)
			angles.y += 360.0f;

		while (angles.y > 180.0f)
			angles.y -= 360.0f;

		angles.z = 0.0f;
	}

	float angle_distance(float firstangle, float secondangle)
	{
		if (firstangle == secondangle) //-V550
			return 0.f;

		bool oppositeSides = false;

		if (firstangle > 0 && secondangle < 0)
			oppositeSides = true;
		else if (firstangle < 0 && secondangle > 0)
			oppositeSides = true;

		if (!oppositeSides)
			return fabs(firstangle - secondangle);

		bool past90 = false;

		if (firstangle > 90 && secondangle < -90)
		{
			firstangle -= (firstangle - 90);
			secondangle += (secondangle + 90);
		}
		else if (firstangle < -90 && secondangle > 90)
		{
			firstangle += (firstangle + 90);
			secondangle -= (secondangle - 90);
		}

		float oneTwo;

		oneTwo = fabs(firstangle - secondangle);

		return oneTwo;
	}

	void normalize_angle(vec3& angles) {
		for (int i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}

	void normalize_vector(vec3& vec3) {
		for (int i = 0; i < 3; i++) {
			while (vec3[i] < -180.0f) vec3[i] += 360.0f;
			while (vec3[i] > 180.0f) vec3[i] -= 360.0f;
		}

		vec3[2] = 0.f;
	}

	//--------------------------------------------------------------------------------
	float normalize_yaw(float f)
	{
		while (f < -180.0f)
			f += 360.0f;

		while (f > 180.0f)
			f -= 360.0f;

		return f;
	}

	float normalize_pitch(float pitch)
	{
		while (pitch > 89.0f)
			pitch -= 180.0f;

		while (pitch < -89.0f)
			pitch += 180.0f;

		return pitch;
	}
	//--------------------------------------------------------------------------------
	float vector_normalize(vec3& v) {
		float l = v.length();

		if (l != 0.0f) //-V550
			v /= l;
		else
		{
			v.x = v.y = 0.0f;
			v.z = 1.0f;
		}

		return l;
	}
	//--------------------------------------------------------------------------------
	void clamp_angles(vec3& angles)
	{
		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		if (angles.y > 180.0f)
			angles.y = 180.0f;
		else if (angles.y < -180.0f)
			angles.y = -180.0f;

		angles.z = 0;
	}
	//--------------------------------------------------------------------------------
	void vector_transform(const vec3& in1, const matrix& in2, vec3& out)
	{
		out = vec3(in1.dot(vec3(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3], in1.dot(vec3(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3], in1.dot(vec3(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3]);
	}
	//--------------------------------------------------------------------------------
	vec3 calculate_angle(const vec3& src, const vec3& dst) {
		vec3 angles;

		vec3 delta = src - dst;
		float hyp = delta.length_2d();

		angles.y = std::atanf(delta.y / delta.x) * M_RADPI;
		angles.x = std::atanf(-delta.z / hyp) * -M_RADPI;
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}
	//--------------------------------------------------------------------------------
	float get_fov(const vec3& view_angle, const vec3& aim_angle)
	{
		vec3 delta = aim_angle - view_angle;
		normalize_angles(delta);

		return min(sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f)), 180.0f);
	}
	//--------------------------------------------------------------------------------
	void angle_vectors(const vec3& angles, vec3& forward)
	{
		float sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	//--------------------------------------------------------------------------------
	void angle_vectors(const vec3& angles, vec3* forward, vec3* right, vec3* up)
	{
		auto sin_cos = [](float radian, float* sin, float* cos)
		{
			*sin = std::sin(radian);
			*cos = std::cos(radian);
		};

		float sp, sy, sr, cp, cy, cr;

		sin_cos(M_PI / 180.0f * angles.x, &sp, &cp);
		sin_cos(M_PI / 180.0f * angles.y, &sy, &cy);
		sin_cos(M_PI / 180.0f * angles.z, &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
			right->y = -1.0f * sr * sp * sy + -1.0f * cr * cy;
			right->z = -1.0f * sr * cp;
		}

		if (up)
		{
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}
	//--------------------------------------------------------------------------------
	void vector_angles(const vec3& forward, vec3& angles)
	{
		vec3 view;

		if (!forward[0] && !forward[1])
		{
			view[0] = 0.0f;
			view[1] = 0.0f;
		}
		else
		{
			view[1] = atan2(forward[1], forward[0]) * 180.0f / M_PI;

			if (view[1] < 0.0f)
				view[1] += 360.0f;

			view[2] = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			view[0] = atan2(forward[2], view[2]) * 180.0f / M_PI;
		}

		angles[0] = -view[0];
		angles[1] = view[1];
		angles[2] = 0.f;
	}
	//--------------------------------------------------------------------------------
	void vector_angles(const vec3& forward, vec3& up, vec3& angles)
	{
		auto left = cross_product(up, forward);
		left.normalize_in_place();

		auto forwardDist = forward.length_2d();

		if (forwardDist > 0.001f)
		{
			angles.x = atan2(-forward.z, forwardDist) * 180.0f / M_PI;
			angles.y = atan2(forward.y, forward.x) * 180.0f / M_PI;

			auto upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2(left.z, upZ) * 180.0f / M_PI;
		}
		else
		{
			angles.x = atan2(-forward.z, forwardDist) * 180.0f / M_PI;
			angles.y = atan2(-left.x, left.y) * 180.0f / M_PI;
			angles.z = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------
	void VectorMAInline(const float* start, float scale, const float* direction, float* dest)
	{
		dest[0] = start[0] + direction[0] * scale;
		dest[1] = start[1] + direction[1] * scale;
		dest[2] = start[2] + direction[2] * scale;
	}

	void VectorMAInline(const vec3& start, float scale, const vec3& direction, vec3& dest)
	{
		dest.x = start.x + direction.x * scale;
		dest.y = start.y + direction.y * scale;
		dest.z = start.z + direction.z * scale;
	}

	void VectorMA(const vec3& start, float scale, const vec3& direction, vec3& dest)
	{
		VectorMAInline(start, scale, direction, dest);
	}

	void VectorMA(const float* start, float scale, const float* direction, float* dest)
	{
		VectorMAInline(start, scale, direction, dest);
	}
	//--------------------------------------------------------------------------------
	vec3 cross_product(const vec3& a, const vec3& b)
	{
		return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	//--------------------------------------------------------------------------------
	float dot_product(const vec3& a, const vec3& b)
	{
		return (a.x * b.x
			+ a.y * b.y
			+ a.z * b.z);
	}
	//--------------------------------------------------------------------------------

	__forceinline const matrix& world_to_screen_matrix()
	{
		static uintptr_t view_matrix = 0;
		if (!view_matrix)
		{
			view_matrix = utils->find_pattern("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9");
			view_matrix = *reinterpret_cast<uintptr_t*>(view_matrix + 0x3) + 176;
		}
		return *reinterpret_cast<matrix*>(view_matrix);
	}

	__forceinline bool screen_transform(const vec3& in, vec3& out)
	{
		const auto& w2_s_matrix = world_to_screen_matrix();
		out.x = w2_s_matrix[0][0] * in[0] + w2_s_matrix[0][1] * in[1] + w2_s_matrix[0][2] * in[2] + w2_s_matrix[0][3];
		out.y = w2_s_matrix[1][0] * in[0] + w2_s_matrix[1][1] * in[1] + w2_s_matrix[1][2] * in[2] + w2_s_matrix[1][3];
		out.z = 0.0f;

		const auto w = w2_s_matrix[3][0] * in.x + w2_s_matrix[3][1] * in.y + w2_s_matrix[3][2] * in.z + w2_s_matrix[3][3];

		if (w < 0.001f)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		const auto invw = 1.0f / w;
		out.x *= invw;
		out.y *= invw;

		return true;
	}

	bool world_to_screen(const vec3& origin, vec3& screen, bool direct)
	{
		if (direct)
		{
			const auto result = screen_transform(origin, screen);

			screen.x = (g->screen.w / 2.0f) + (screen.x * g->screen.w) / 2.0f;
			screen.y = (g->screen.h / 2.0f) - (screen.y * g->screen.h) / 2.0f;

			return result;
		}
		else {
			float w;

			const matrix& matrix = interfaces->engine->world_to_screen_matrix();

			// check if it's in view first.
			// note - dex; w is below 0 when world position is around -90 / +90 from the player's camera on the y axis.
			w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];
			if (w < 0.001f)
				return false;

			// calculate x and y.
			screen.x = matrix[0][0] * origin.x + matrix[0][1] * origin.y + matrix[0][2] * origin.z + matrix[0][3];
			screen.y = matrix[1][0] * origin.x + matrix[1][1] * origin.y + matrix[1][2] * origin.z + matrix[1][3];

			screen /= w;

			// calculate screen position.
			screen.x = (g->screen.w / 2) + (screen.x * g->screen.w) / 2;
			screen.y = (g->screen.h / 2) - (screen.y * g->screen.h) / 2;

			return true;
		}
	}
	//--------------------------------------------------------------------------------
	void SmoothAngle(vec3& From, vec3& To, float Percent)
	{
		vec3 VecDelta = From - To;
		math::normalize_angles(VecDelta);
		VecDelta.x *= Percent;
		VecDelta.y *= Percent;
		To = From - VecDelta;
	}
	//--------------------------------------------------------------------------------
	void rotate_triangle(std::array<vec2, 3>& points, float rotation)
	{
		const auto pointsCenter = (points.at(0) + points.at(1) + points.at(2)) / 3;
		for (auto& point : points)
		{
			point -= pointsCenter;

			const auto tempX = point.x;
			const auto tempY = point.y;

			const auto theta = DEG2RAD(rotation);
			const auto c = cos(theta);
			const auto s = sin(theta);

			point.x = tempX * c - tempY * s;
			point.y = tempX * s + tempY * c;

			point += pointsCenter;
		}
	}
	//--------------------------------------------------------------------------------
	float random_float(float min, float max) {
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat");
		return m_RandomFloat(min, max);
	}
	//--------------------------------------------------------------------------------
	int random_int(int min, int max) {
		typedef int(*RandomInt_t)(int, int);
		static RandomInt_t m_RandomInt = (RandomInt_t)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomInt");
		return m_RandomInt(min, max);
	}
	//--------------------------------------------------------------------------------
	void random_seed(int seed) {
		typedef void(*RandomSeed_t)(int);
		static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed");
		return m_RandomSeed(seed);
	}
	//--------------------------------------------------------------------------------
	float segment_to_segment(const vec3& s1, const vec3& s2, const vec3& k1, const vec3& k2)
	{
		static auto constexpr epsilon = 0.00000001f;

		auto u = s2 - s1;
		auto v = k2 - k1;
		auto w = s1 - k1;

		auto a = u.dot(u); //-V525
		auto b = u.dot(v);
		auto c = v.dot(v);
		auto d = u.dot(w);
		auto e = v.dot(w);
		auto D = a * c - b * b;

		auto sn = 0.0f, sd = D;
		auto tn = 0.0f, td = D;

		if (D < epsilon)
		{
			sn = 0.0f;
			sd = 1.0f;
			tn = e;
			td = c;
		}
		else
		{
			sn = b * e - c * d;
			tn = a * e - b * d;

			if (sn < 0.0f)
			{
				sn = 0.0f;
				tn = e;
				td = c;
			}
			else if (sn > sd)
			{
				sn = sd;
				tn = e + b;
				td = c;
			}
		}

		if (tn < 0.0f)
		{
			tn = 0.0f;

			if (-d < 0.0f)
				sn = 0.0f;
			else if (-d > a)
				sn = sd;
			else
			{
				sn = -d;
				sd = a;
			}
		}
		else if (tn > td)
		{
			tn = td;

			if (-d + b < 0.0f)
				sn = 0.0f;
			else if (-d + b > a)
				sn = sd;
			else
			{
				sn = -d + b;
				sd = a;
			}
		}

		auto sc = fabs(sn) < epsilon ? 0.0f : sn / sd;
		auto tc = fabs(tn) < epsilon ? 0.0f : tn / td;

		auto dp = w + u * sc - v * tc;
		return dp.length();
	}
	//--------------------------------------------------------------------------------
	bool intersect_line_with_bb(vec3& start, vec3& end, vec3& min, vec3& max)
	{
		char quadrant[3];
		int i;

		vec3 candidatePlane;
		for (i = 0; i < 3; i++) {
			if (start[i] < min[i]) {
				quadrant[i] = 1;
				candidatePlane[i] = min[i];
			}
			else if (start[i] > max[i]) {
				quadrant[i] = 0;
				candidatePlane[i] = max[i];
			}
			else {
				quadrant[i] = 2;
			}
		}

		// Calculate T distances to candidate planes
		vec3 maxT;
		for (i = 0; i < 3; i++) {
			if (quadrant[i] != 2 && end[i] != 0.f) //-V550
				maxT[i] = (candidatePlane[i] - start[i]) / end[i];
			else
				maxT[i] = -1.f;
		}

		// Get largest of the maxT's for final choice of intersection
		int whichPlane = 0;
		for (i = 1; i < 3; i++) {
			if (maxT[whichPlane] < maxT[i])
				whichPlane = i;
		}

		if (maxT[whichPlane] < 0.0f)
			return false;

		for (i = 0; i < 3; i++) {
			if (whichPlane != i) {
				float temp = start[i] + maxT[whichPlane] * end[i];
				if (temp < min[i] || temp > max[i]) {
					return false;
				}
			}
		}

		return true;
	}
	//--------------------------------------------------------------------------------
	vec3 vector_rotate(const vec3& in1, const matrix& in2)
	{
		return vec3(in1.dot(in2[0]), in1.dot(in2[1]), in1.dot(in2[2]));
	}
	//--------------------------------------------------------------------------------
	vec3 vector_rotate(const vec3& in1, const vec3& in2)
	{
		const auto matrix = angle_matrix(in2);
		return vector_rotate(in1, matrix);
	}
	//--------------------------------------------------------------------------------
	matrix angle_matrix(const vec3& angles)
	{
		matrix result;

		m128 angle, sin, cos;
		angle.f[0] = DEG2RAD(angles.x);
		angle.f[1] = DEG2RAD(angles.y);
		angle.f[2] = DEG2RAD(angles.z);
		sincos_ps(angle.v, &sin.v, &cos.v);

		result[0][0] = cos.f[0] * cos.f[1];
		result[1][0] = cos.f[0] * sin.f[1];
		result[2][0] = -sin.f[0];

		const auto crcy = cos.f[2] * cos.f[1];
		const auto crsy = cos.f[2] * sin.f[1];
		const auto srcy = sin.f[2] * cos.f[1];
		const auto srsy = sin.f[2] * sin.f[1];

		result[0][1] = sin.f[0] * srcy - crsy;
		result[1][1] = sin.f[0] * srsy + crcy;
		result[2][1] = sin.f[2] * cos.f[0];

		result[0][2] = sin.f[0] * crcy + srsy;
		result[1][2] = sin.f[0] * crsy - srcy;
		result[2][2] = cos.f[2] * cos.f[0];

		return result;
	}
	//
	void angle_matrix(const vec3& ang, const vec3& pos, matrix& out)
	{
		out = angle_matrix(ang);
		out.set_origin(pos);
	}
	//--------------------------------------------------------------------------------
	void vector_rotate(const vec3& in1, const matrix& in2, vec3& out)
	{
		out.x = in1.x * in2[0][0] + in1.y * in2[1][0] + in1.z * in2[2][0];
		out.y = in1.x * in2[0][1] + in1.y * in2[1][1] + in1.z * in2[2][1];
		out.z = in1.x * in2[0][2] + in1.y * in2[1][2] + in1.z * in2[2][2];
	}
	//--------------------------------------------------------------------------------
	vec3 matrix_get_origin(const matrix& src)
	{
		return vec3(src[0][3], src[1][3], src[2][3]);
	}
	//--------------------------------------------------------------------------------
	void matrix_set_origin(const vec3& pos, matrix& matrix)
	{
		for (auto i = 0; i < 3; i++)
			matrix[i][3] = pos[i];
	}
	//--------------------------------------------------------------------------------
	void fast_rsqrt(float a, float* out)
	{
		const auto xx = _mm_load_ss(&a);
		auto xr = _mm_rsqrt_ss(xx);
		auto xt = _mm_mul_ss(xr, xr);
		xt = _mm_mul_ss(xt, xx);
		xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
		xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
		xr = _mm_mul_ss(xr, xt);
		_mm_store_ss(out, xr);
	}
	//--------------------------------------------------------------------------------
	float fast_vec_normalize(vec3& vec)
	{
		const auto sqrlen = vec.length_sqr() + 1.0e-10f;
		float invlen;
		fast_rsqrt(sqrlen, &invlen);
		vec.x *= invlen;
		vec.y *= invlen;
		vec.z *= invlen;
		return sqrlen * invlen;
	}
	//--------------------------------------------------------------------------------
	void math::matrix_copy(const matrix& in, matrix& out)
	{
		memcpy(out.Base(), in.Base(), sizeof(matrix));
	}
	//--------------------------------------------------------------------------------
	void math::concat_transforms(const matrix& in1, const matrix& in2, matrix& out)
	{
		if (&in1 == &out)
		{
			matrix in1b;
			matrix_copy(in1, in1b);
			concat_transforms(in1b, in2, out);
			return;
		}

		if (&in2 == &out)
		{
			matrix in2b;
			matrix_copy(in2, in2b);
			concat_transforms(in1, in2b, out);
			return;
		}

		out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
		out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
		out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
		out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] + in1[0][2] * in2[2][3] + in1[0][3];

		out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
		out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
		out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
		out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] + in1[1][2] * in2[2][3] + in1[1][3];

		out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
		out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
		out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
		out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] + in1[2][2] * in2[2][3] + in1[2][3];
	}
	//--------------------------------------------------------------------------------
	double fast_sin(double x)
	{
		int k;
		double y;
		double z;

		z = x;
		z *= 0.3183098861837907;
		z += 6755399441055744.0;
		k = *((int*)&z);
		z = k;
		z *= 3.1415926535897932;
		x -= z;
		y = x;
		y *= x;
		z = 0.0073524681968701;
		z *= y;
		z -= 0.1652891139701474;
		z *= y;
		z += 0.9996919862959676;
		x *= z;
		k &= 1;
		k += k;
		z = k;
		z *= x;
		x -= z;

		return x;
	}
	//--------------------------------------------------------------------------------
	float simple_spline(float value)
	{
		auto value_squared = value * value;
		return 3.0f * value_squared - 2.0f * value_squared * value;
	}
	//--------------------------------------------------------------------------------
	float simple_spline_remap_val_clamped(float value, float a, float b, float c, float d)
	{
		if (a == b) //-V550
			return value >= b ? d : c;

		auto c_value = math::clamp((value - a) / (b - a), 0.0f, 1.0f);
		return c + (d - c) * simple_spline(c_value);
	}
	//--------------------------------------------------------------------------------
	float direction_difference(const vec3& first, const vec3& second)
	{
		auto a = math::normalize_yaw(RAD2DEG(atan2(first.y, first.x)));
		auto b = math::normalize_yaw(RAD2DEG(atan2(second.y, second.x)));

		auto c = fmodf(a - b, 360.0f);

		if (a > b)
		{
			if (c >= 180.f)
				c -= 360.f;
		}
		else
		{
			if (c <= -180.f)
				c += 360.f;
		}

		return c;
	}
}