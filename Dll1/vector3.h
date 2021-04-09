#pragma once
#include <sstream>
#include <DirectXMath.h>

constexpr auto rad_pi = 3.14159265358979323846;
constexpr auto deg_pi = 180.0;

template<typename T>
T to_rad(T degrees)
{
	return (degrees * (static_cast<T>(rad_pi) / static_cast<T>(deg_pi)));
}

template<typename T>
T to_deg(T radians)
{
	return (radians * (static_cast<T>(deg_pi) / static_cast<T>(rad_pi)));
}

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

#define vec_zero vec3(0.0f, 0.0f, 0.0f)

class quaternion_t
{
public:
	float x, y, z, w;
};

typedef float vec_t;
// 3D Vector4D
class vec4 //-V690
{
public:
	// Members
	vec_t x, y, z, w;

	// Construction/destruction:
	vec4(void);
	vec4(vec_t X, vec_t Y, vec_t Z, vec_t W);
	vec4(vec_t* clr);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const vec4& v) const;
	bool operator!=(const vec4& v) const;

	// arithmetic operations
	vec4& operator+=(const vec4& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	vec4& operator-=(const vec4& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	vec4& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	vec4& operator*=(const vec4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	vec4& operator/=(const vec4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	// this ought to be an opcode.
	vec4& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	// this ought to be an opcode.
	vec4& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	vec4& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	vec_t Length() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance&&
			w > -tolerance && w < tolerance);
	}

	vec_t NormalizeInPlace();
	vec4 Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(vec4 const& boxmin, vec4 const& boxmax);

	// Get the distance from this vector to the other one.
	vec_t DistTo(const vec4& vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const vec4& vOther) const
	{
		vec4 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;
		delta.w = w - vOther.w;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const vec4& a, const vec4& b, float scalar);

	// Dot product.
	vec_t Dot(const vec4& vOther) const;

	// assignment
	vec4& operator=(const vec4& vOther);

	// 2d
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	vec4  ProjectOnto(const vec4& onto);

	// copy constructors
	// Vector4D(const Vector4D &vOther);

	// arithmetic operations
	vec4 operator-(void) const;

	vec4 operator+(const vec4& v) const;
	vec4 operator-(const vec4& v) const;
	vec4 operator*(const vec4& v) const;
	vec4 operator/(const vec4& v) const;
	vec4 operator*(float fl) const;
	vec4 operator/(float fl) const;

	// Returns a vector with the min or max in X, Y, and Z.
	vec4 Min(const vec4& vOther) const;
	vec4 Max(const vec4& vOther) const;
};

class vec3
{
public:
	__inline vec3(void)
	{
		invalidate();
	}

	vec3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	vec3(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix;
		y = iy;
		z = iz;
	}

	void mul(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	vec3 clamp()
	{
		if (x < -89.0f)
			x = -89.0f;

		if (x > 89.0f)
			x = 89.0f;

		while (y < -180.0f)
			y += 360.0f;

		while (y > 180.0f)
			y -= 360.0f;

		z = 0.0f;
		return *this;
	}

	void mul_add(const vec3& a, const vec3& b, float scalar)
	{
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	bool is_valid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	void invalidate()
	{
		x = y = z = FLT_MAX;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	bool __inline is_zero()
	{
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	void __inline set_to_zero()
	{
		x = y = z = 0.0f;
	}

	bool operator==(const vec3& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool operator!=(const vec3& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	vec3& operator+=(const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3& operator-=(const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;

		return *this;
	}

	vec3& operator*=(const vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	vec3& operator/=(const vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}

	vec3& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;

		return *this;
	}

	vec3& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;

		return *this;
	}

	vec3& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;

		return *this;
	}

	void normalize_in_place()
	{
		vec3& v = *this;

		float iradius = 1.0f / (this->length() + FLT_EPSILON);

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;
	}

	vec3 normalized() const
	{
		vec3 res = *this;
		float l = res.length();

		if (l)  //-V550
			res /= l;
		else
			res.x = res.y = res.z = 0.0f;

		return res;
	}

	float normalize() const
	{
		vec3 res = *this;
		float l = res.length();

		if (l)
			res /= l;
		else
			res.x = res.y = res.z = 0.0f;

		return l;
	}

	void normalize_no_clamp()
	{
		this->x -= floorf(this->x / 360.0f + 0.5f) * 360.0f;
		this->y -= floorf(this->y / 360.0f + 0.5f) * 360.0f;
		this->z -= floorf(this->z / 360.0f + 0.5f) * 360.0f;
	}

	float dist_to(const vec3& vOther) const
	{
		vec3 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.length();
	}

	float dist_to_sqr(const vec3& vOther) const
	{
		vec3 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.length_sqr();
	}

	float dot(const vec3& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}

	void cross_product(const vec3& a, const vec3& b, vec3& result)
	{
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}

	vec3 cross(const vec3& vOther)
	{
		vec3 res;
		cross_product(*this, vOther, res);

		return res;
	}

	vec3 direction()
	{
		return vec3(cos(y * 3.141592654f / 180.0f) * cos(x * 3.141592654f / 180.0f), sin(y * 3.141592654f / 180.0f) * cos(x * 3.141592654f / 180.0f), sin(-x * 3.141592654f / 180.0f)).normalized();
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	float length_sqr() const
	{
		return x * x + y * y + z * z;
	}

	float length_2d() const
	{
		return sqrt(x * x + y * y);
	}

	float length_2d_sqr() const
	{
		return x * x + y * y;
	}

	vec3 to_euler(vec3* pseudoup = nullptr)
	{
		auto pitch = 0.0f;
		auto yaw = 0.0f;
		auto roll = 0.0f;

		auto length = length_2d();

		if (pseudoup)
		{
			auto left = pseudoup->cross(*this);

			left.normalize();

			pitch = to_deg(std::atan2(-this->z, length));

			if (pitch < 0.0f)
				pitch += 360.0f;

			if (length > 0.001f) {
				yaw = to_deg(std::atan2(this->y, this->x));

				if (yaw < 0.0f)
					yaw += 360.0f;

				auto up_z = (this->x * left.y) - (this->y * left.x);

				roll = to_deg(std::atan2(left.z, up_z));

				if (roll < 0.0f)
					roll += 360.0f;
			}
			else {
				yaw = to_deg(std::atan2(-left.x, left.y));

				if (yaw < 0.0f)
					yaw += 360.0f;
			}
		}
		else {
			if (this->x == 0.0f && this->y == 0.0f) {
				if (this->z > 0.0f)
					pitch = 270.0f;
				else
					pitch = 90.0f;
			}
			else {
				pitch = to_deg(std::atan2(-this->z, length));

				if (pitch < 0.0f)
					pitch += 360.0f;

				yaw = to_deg(std::atan2(this->y, this->x));

				if (yaw < 0.0f)
					yaw += 360.0f;
			}
		}

		return { pitch, yaw, roll };
	}

	vec3& operator=(const vec3& vOther)
	{
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;

		return *this;
	}

	bool operator < (const vec3& v) {
		return { this->x < v.x&&
				 this->y < v.y&&
				 this->z < v.z };
	}

	bool operator > (const vec3& v) {
		return { this->x > v.x &&
				 this->y > v.y &&
				 this->z > v.z };
	}

	bool operator<=(const vec3& v) {
		return { this->x <= v.x &&
				 this->y <= v.y &&
				 this->z <= v.z };
	}

	bool operator>=(const vec3& v) {
		return { this->x >= v.x &&
				 this->y >= v.y &&
				 this->z >= v.z };
	}

	vec3 operator-(void) const
	{
		return vec3(-x, -y, -z);
	}

	vec3 operator+(const vec3& v) const
	{
		return vec3(x + v.x, y + v.y, z + v.z);
	}

	vec3 operator+(float fl) const
	{
		return vec3(x + fl, y + fl, z + fl);
	}

	vec3 operator-(const vec3& v) const
	{
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	vec3 operator-(float fl) const
	{
		return vec3(x - fl, y - fl, z - fl);
	}

	vec3 operator*(float fl) const
	{
		return vec3(x * fl, y * fl, z * fl);
	}

	vec3 operator*(const vec3& v) const
	{
		return vec3(x * v.x, y * v.y, z * v.z);
	}

	vec3 operator/(float fl) const
	{
		return vec3(x / fl, y / fl, z / fl);
	}

	vec3 operator/(const vec3& v) const
	{
		return vec3(x / v.x, y / v.y, z / v.z);
	}

	float x, y, z;
};

inline vec3 operator*(float lhs, const vec3& rhs)
{
	return rhs * lhs;
}

inline vec3 operator/(float lhs, const vec3& rhs)
{
	return rhs / lhs;
}

class __declspec(align(16)) vector_aligned : public vec3
{
public:

	inline vector_aligned(void)
	{
		w = 0.0f;
	};

	inline vector_aligned(float X, float Y, float Z)
	{
		w = 0.0f;
		init(X, Y, Z);
	}

public:

	explicit vector_aligned(const vec3& vOther)
	{
		w = 0.0f;
		init(vOther.x, vOther.y, vOther.z);
	}

	vector_aligned& operator=(const vec3& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	vector_aligned& operator=(const vector_aligned& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};

typedef unsigned __int32 uint32;

inline uint32 const float_bits(const float& f)
{
	union Convertor_t
	{
		float f;
		uint32 ul;
	} tmp;

	tmp.f = f;
	return tmp.ul;
}

inline bool is_finite(const float& f)
{
	return ((float_bits(f) & 0x7F800000) != 0x7F800000);
}

inline void vector_multiply(const vec3& a, float b, vec3& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

inline void vector_ma(const vec3& start, float scale, const vec3& direction, vec3& dest)
{
	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}

inline void vector_add(const vec3& a, const vec3& b, vec3& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void vector_substract(const vec3& a, const vec3& b, vec3& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

class matrix
{
public:
	matrix() {}
	matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init(const vec3& xAxis, const vec3& yAxis, const vec3& zAxis, const vec3& vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix(const vec3& xAxis, const vec3& yAxis, const vec3& zAxis, const vec3& vecOrigin)
	{
		init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void set_origin(vec3 const& p)
	{
		m_flMatVal[0][3] = p.x;
		m_flMatVal[1][3] = p.y;
		m_flMatVal[2][3] = p.z;
	}
	inline vec3 get_origin()
	{
		return vec3(m_flMatVal[0][3], m_flMatVal[1][3], m_flMatVal[2][3]);
	}
	inline void invalidate(void)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				m_flMatVal[i][j] = std::numeric_limits<float>::infinity();
			}
		}
	}
	vec3 at(int i) const { return vec3{ m_flMatVal[0][i], m_flMatVal[1][i], m_flMatVal[2][i] }; }
	float* operator[](int i) { return m_flMatVal[i]; }
	const float* operator[](int i) const { return m_flMatVal[i]; }
	float* Base() { return &m_flMatVal[0][0]; }
	const float* Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};