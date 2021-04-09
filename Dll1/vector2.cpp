#include "vector2.h"

vec2::vec2(void)
{
    x = y = 0.0f;
}

vec2::vec2(vec_t X, vec_t Y)
{
    x = X; y = Y;
}

vec2::vec2(vec_t* clr)
{
    x = clr[0]; y = clr[1];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void vec2::Init(vec_t ix, vec_t iy)
{
    x = ix; y = iy;
}

void vec2::Random(float minVal, float maxVal)
{
    x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
    y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

vec_t Vector2DLengthDrop(const vec2& v)
{
    return (vec_t)sqrt(v.x * v.x + v.y * v.y);
}

vec_t vec2::LengthDrop(void) const
{
    return Vector2DLengthDrop(*this);
}


void Vector2DClear(vec2& a)
{
    a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

vec2& vec2::operator=(const vec2& vOther)
{
    x = vOther.x; y = vOther.y;
    return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

vec_t& vec2::operator[](int i)
{
    return ((vec_t*)this)[i];
}

vec_t vec2::operator[](int i) const
{
    return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

vec_t* vec2::Base()
{
    return (vec_t*)this;
}

vec_t const* vec2::Base() const
{
    return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool vec2::IsValid() const
{
    return !isinf(x) && !isinf(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool vec2::operator==(const vec2& src) const
{
    return (src.x == x) && (src.y == y);
}

bool vec2::operator!=(const vec2& src) const
{
    return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

void Vector2DCopy(const vec2& src, vec2& dst)
{
    dst.x = src.x;
    dst.y = src.y;
}

void vec2::CopyToArray(float* rgfl) const
{
    rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void vec2::Negate()
{
    x = -x; y = -y;
}

void Vector2DAdd(const vec2& a, const vec2& b, vec2& c)
{
    c.x = a.x + b.x;
    c.y = a.y + b.y;
}

void Vector2DSubtract(const vec2& a, const vec2& b, vec2& c)
{
    c.x = a.x - b.x;
    c.y = a.y - b.y;
}

void Vector2DMultiply(const vec2& a, vec_t b, vec2& c)
{
    c.x = a.x * b;
    c.y = a.y * b;
}

void Vector2DMultiply(const vec2& a, const vec2& b, vec2& c)
{
    c.x = a.x * b.x;
    c.y = a.y * b.y;
}


void Vector2DDivide(const vec2& a, vec_t b, vec2& c)
{
    vec_t oob = 1.0f / b;
    c.x = a.x * oob;
    c.y = a.y * oob;
}

void Vector2DDivide(const vec2& a, const vec2& b, vec2& c)
{
    c.x = a.x / b.x;
    c.y = a.y / b.y;
}

void Vector2DMA(const vec2& start, float s, const vec2& dir, vec2& result)
{
    result.x = start.x + s * dir.x;
    result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
void vec2::MulAdd(const vec2& a, const vec2& b, float scalar)
{
    x = a.x + b.x * scalar;
    y = a.y + b.y * scalar;
}

void Vector2DLerp(const vec2& src1, const vec2& src2, vec_t t, vec2& dest)
{
    dest[0] = src1[0] + (src2[0] - src1[0]) * t;
    dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
vec_t DotProduct2D(const vec2& a, const vec2& b)
{
    return(a.x * b.x + a.y * b.y);
}

// for backwards compatability
vec_t vec2::Dot(const vec2& vOther) const
{
    return DotProduct2D(*this, vOther);
}

vec_t Vector2DNormalize(vec2& v)
{
    vec_t l = v.Length();
    if (l != 0.0f) {
        v /= l;
    }
    else {
        v.x = v.y = 0.0f;
    }
    return l;
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
vec_t Vector2DLength(const vec2& v) //-V524
{
    return (vec_t)sqrt(v.x * v.x + v.y * v.y);
}

vec_t vec2::NormalizeInPlace()
{
    return Vector2DNormalize(*this);
}

bool vec2::IsLengthGreaterThan(float val) const
{
    return LengthSqr() > val * val;
}

bool vec2::IsLengthLessThan(float val) const
{
    return LengthSqr() < val * val;
}

vec_t vec2::Length(void) const
{
    return Vector2DLength(*this);
}


void Vector2DMin(const vec2& a, const vec2& b, vec2& result)
{
    result.x = (a.x < b.x) ? a.x : b.x;
    result.y = (a.y < b.y) ? a.y : b.y;
}


void Vector2DMax(const vec2& a, const vec2& b, vec2& result)
{
    result.x = (a.x > b.x) ? a.x : b.x;
    result.y = (a.y > b.y) ? a.y : b.y;
}

//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
void ComputeClosestPoint2D(const vec2& vecStart, float flMaxDist, const vec2& vecTarget, vec2* pResult)
{
    vec2 vecDelta;
    Vector2DSubtract(vecTarget, vecStart, vecDelta);
    float flDistSqr = vecDelta.LengthSqr();
    if (flDistSqr <= flMaxDist * flMaxDist) {
        *pResult = vecTarget;
    }
    else {
        vecDelta /= sqrt(flDistSqr);
        Vector2DMA(vecStart, flMaxDist, vecDelta, *pResult);
    }
}

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

vec2 vec2::Min(const vec2& vOther) const
{
    return vec2(x < vOther.x ? x : vOther.x, y < vOther.y ? y : vOther.y);
}

vec2 vec2::Max(const vec2& vOther) const
{
    return vec2(x > vOther.x ? x : vOther.x, y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

vec2 vec2::operator-(void) const
{
    return vec2(-x, -y);
}

vec2 vec2::operator+(const vec2& v) const
{
    vec2 res;
    Vector2DAdd(*this, v, res);
    return res;
}

vec2 vec2::operator-(const vec2& v) const
{
    vec2 res;
    Vector2DSubtract(*this, v, res);
    return res;
}

vec2 vec2::operator*(float fl) const
{
    vec2 res;
    Vector2DMultiply(*this, fl, res);
    return res;
}

vec2 vec2::operator*(const vec2& v) const
{
    vec2 res;
    Vector2DMultiply(*this, v, res);
    return res;
}

vec2 vec2::operator/(float fl) const
{
    vec2 res;
    Vector2DDivide(*this, fl, res);
    return res;
}

vec2 vec2::operator/(const vec2& v) const
{
    vec2 res;
    Vector2DDivide(*this, v, res);
    return res;
}

vec2 operator*(float fl, const vec2& v)
{
    return v * fl;
}