#include "vector3.h"

void VectorCopy(const vec4& src, vec4& dst)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}
void VectorLerp(const vec4& src1, const vec4& src2, vec_t t, vec4& dest)
{
    dest.x = src1.x + (src2.x - src1.x) * t;
    dest.y = src1.y + (src2.y - src1.y) * t;
    dest.z = src1.z + (src2.z - src1.z) * t;
    dest.w = src1.w + (src2.w - src1.w) * t;
}
float VectorLength(const vec4& v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec_t NormalizeVector(vec4& v)
{
    vec_t l = v.Length();
    if (l != 0.0f) {
        v /= l;
    }
    else {
        v.x = v.y = v.z = v.w = 0.0f;
    }
    return l;
}

vec4::vec4(void)
{
    Invalidate();
}
vec4::vec4(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
    x = X;
    y = Y;
    z = Z;
    w = W;
}
vec4::vec4(vec_t* clr)
{
    x = clr[0];
    y = clr[1];
    z = clr[2];
    w = clr[3];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void vec4::Init(vec_t ix, vec_t iy, vec_t iz, vec_t iw)
{
    x = ix; y = iy; z = iz; w = iw;
}

void vec4::Random(vec_t minVal, vec_t maxVal)
{
    x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    w = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
void vec4::Zero()
{
    x = y = z = w = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

vec4& vec4::operator=(const vec4& vOther)
{
    x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
    return *this;
}


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
vec_t& vec4::operator[](int i)
{
    return ((vec_t*)this)[i];
}

vec_t vec4::operator[](int i) const
{
    return ((vec_t*)this)[i];
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
vec_t* vec4::Base()
{
    return (vec_t*)this;
}

vec_t const* vec4::Base() const
{
    return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool vec4::IsValid() const
{
    return !isinf(x) && !isinf(y) && !isinf(z) && !isinf(w);
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

void vec4::Invalidate()
{
    //#ifdef _DEBUG
    //#ifdef VECTOR_PARANOIA
    x = y = z = w = std::numeric_limits<float>::infinity();
    //#endif
    //#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool vec4::operator==(const vec4& src) const
{
    return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

bool vec4::operator!=(const vec4& src) const
{
    return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
void vec4::CopyToArray(float* rgfl) const
{
    rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; rgfl[3] = w;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void vec4::Negate()
{
    x = -x; y = -y; z = -z; w = -w;
}

// Get the component of this vector parallel to some other given vector
vec4 vec4::ProjectOnto(const vec4& onto)
{
    return onto * (this->Dot(onto) / (onto.LengthSqr()));
}

// FIXME: Remove
// For backwards compatability
void vec4::MulAdd(const vec4& a, const vec4& b, float scalar)
{
    x = a.x + b.x * scalar;
    y = a.y + b.y * scalar;
    z = a.z + b.z * scalar;
    w = a.w + b.w * scalar;
}

vec4 VectorLerp(const vec4& src1, const vec4& src2, vec_t t)
{
    vec4 result;
    VectorLerp(src1, src2, t, result);
    return result;
}

vec_t vec4::Dot(const vec4& b) const
{
    return (x * b.x + y * b.y + z * b.z + w * b.w);
}
void VectorClear(vec4& a)
{
    a.x = a.y = a.z = a.w = 0.0f;
}

vec_t vec4::Length(void) const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

// check a point against a box
bool vec4::WithinAABox(vec4 const& boxmin, vec4 const& boxmax)
{
    return (
        (x >= boxmin.x) && (x <= boxmax.x) &&
        (y >= boxmin.y) && (y <= boxmax.y) &&
        (z >= boxmin.z) && (z <= boxmax.z) &&
        (w >= boxmin.w) && (w <= boxmax.w)
        );
}

//-----------------------------------------------------------------------------
// Get the distance from this vector to the other one 
//-----------------------------------------------------------------------------
vec_t vec4::DistTo(const vec4& vOther) const
{
    vec4 delta;
    delta = *this - vOther;
    return delta.Length();
}

//-----------------------------------------------------------------------------
// Returns a vector with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------
vec4 vec4::Min(const vec4& vOther) const
{
    return vec4(x < vOther.x ? x : vOther.x,
        y < vOther.y ? y : vOther.y,
        z < vOther.z ? z : vOther.z,
        w < vOther.w ? w : vOther.w);
}

vec4 vec4::Max(const vec4& vOther) const
{
    return vec4(x > vOther.x ? x : vOther.x,
        y > vOther.y ? y : vOther.y,
        z > vOther.z ? z : vOther.z,
        w > vOther.w ? w : vOther.w);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

vec4 vec4::operator-(void) const
{
    return vec4(-x, -y, -z, -w);
}

vec4 vec4::operator+(const vec4& v) const
{
    return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4 vec4::operator-(const vec4& v) const
{
    return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4 vec4::operator*(float fl) const
{
    return vec4(x * fl, y * fl, z * fl, w * fl);
}

vec4 vec4::operator*(const vec4& v) const
{
    return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

vec4 vec4::operator/(float fl) const
{
    return vec4(x / fl, y / fl, z / fl, w / fl);
}

vec4 vec4::operator/(const vec4& v) const
{
    return vec4(x / v.x, y / v.y, z / v.z, w / v.w);
}

vec4 operator*(float fl, const vec4& v)
{
    return v * fl;
}