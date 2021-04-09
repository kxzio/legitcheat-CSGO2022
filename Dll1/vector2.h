#pragma once
#include <math.h>
#include <iostream>

typedef float vec_t;
// 2D Vector
class vec2
{
public:
    // Members
    vec_t x, y;

    // Construction/destruction:
    vec2(void);
    vec2(vec_t X, vec_t Y);
    vec2(vec_t* clr);

    vec2(const vec2& vOther)
    {
        x = vOther.x; y = vOther.y;
    }

    // Initialization
    void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
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
    bool operator==(const vec2& v) const;
    bool operator!=(const vec2& v) const;

    // arithmetic operations
    vec2& operator+=(const vec2& v)
    {
        x += v.x; y += v.y;
        return *this;
    }

    vec2& operator-=(const vec2& v)
    {
        x -= v.x; y -= v.y;
        return *this;
    }

    vec2& operator*=(float fl)
    {
        x *= fl;
        y *= fl;
        return *this;
    }

    vec2& operator*=(const vec2& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    vec2& operator/=(const vec2& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    // this ought to be an opcode.
    vec2& operator+=(float fl)
    {
        x += fl;
        y += fl;
        return *this;
    }

    // this ought to be an opcode.
    vec2& operator/=(float fl)
    {
        x /= fl;
        y /= fl;
        return *this;
    }
    vec2& operator-=(float fl)
    {
        x -= fl;
        y -= fl;
        return *this;
    }

    // negate the vector components
    void Negate();

    // Get the vector's magnitude.
    vec_t Length() const;

    // Get the vector's magnitude squared.
    vec_t LengthSqr(void) const
    {
        return (x * x + y * y);
    }

    // return true if this vector is (0,0,0) within tolerance
    bool IsZero(float tolerance = 0.01f) const
    {
        return (x > -tolerance && x < tolerance&&
            y > -tolerance && y < tolerance);
    }

    vec_t NormalizeInPlace();
    vec2 Normalized() const;
    bool IsLengthGreaterThan(float val) const;
    bool IsLengthLessThan(float val) const;

    // check if a vector is within the box defined by two other vectors
    bool WithinAABox(vec2 const& boxmin, vec2 const& boxmax);

    // Get the distance from this vector to the other one.
    vec_t DistTo(const vec2& vOther) const;

    // Get the distance from this vector to the other one squared.
    // NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
    // may be able to tidy this up after switching to VC7
    vec_t DistToSqr(const vec2& vOther) const
    {
        vec2 delta;

        delta.x = x - vOther.x;
        delta.y = y - vOther.y;

        return delta.LengthSqr();
    }

    // Copy
    void CopyToArray(float* rgfl) const;

    // Multiply, add, and assign to this (ie: *this = a + b * scalar). This
    // is about 12% faster than the actual vector equation (because it's done per-component
    // rather than per-vector).
    void MulAdd(const vec2& a, const vec2& b, float scalar);

    // Dot product.
    vec_t Dot(const vec2& vOther) const;

    // assignment
    vec2& operator=(const vec2& vOther);

    // 2d
    vec_t Length2D(void) const;
    vec_t Length2DSqr(void) const;
    vec_t LengthDrop() const;
    vec2 NormalizedDrop() const
    {
        vec2 reses = *this;
        float l = reses.LengthDrop();
        if (l != 0.0f)  //-V550
            reses /= l;
        else
            reses.x = reses.y = 0.0f;
        return reses;
    }
    vec_t DistToDrop(const vec2& vOther) const
    {
        vec2 deltar;

        deltar.x = x - vOther.x;
        deltar.y = y - vOther.y;

        return deltar.LengthDrop();
    }

    /// Get the component of this vector parallel to some other given vector
    vec2  ProjectOnto(const vec2& onto);

    // copy constructors
    // Vector2D(const Vector2D &vOther);

    // arithmetic operations
    vec2 operator-(void) const;

    vec2 operator+(const vec2& v) const;
    vec2 operator-(const vec2& v) const;
    vec2 operator*(const vec2& v) const;
    vec2 operator/(const vec2& v) const;
    vec2 operator*(float fl) const;
    vec2 operator/(float fl) const;

    // Cross product between two vectors.
    vec2 Cross(const vec2& vOther) const;

    // Returns a vector with the min or max in X, Y, and Z.
    vec2 Min(const vec2& vOther) const;
    vec2 Max(const vec2& vOther) const;
};

