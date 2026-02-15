#pragma once

#include <math.h>
#include <d3d8types.h> // For D3DCOLOR, etc if needed, or just define them. 
// d3d8types.h might be in min-dx8-sdk.

#ifndef D3DXINLINE
#ifdef __cplusplus
#define D3DXINLINE inline
#else
#define D3DXINLINE
#endif
#endif

// Vectors
typedef struct D3DXVECTOR2 {
    float x, y;

    D3DXVECTOR2() {}
    D3DXVECTOR2(const float *pf) { x = pf[0]; y = pf[1]; }
    D3DXVECTOR2(float fx, float fy) { x = fx; y = fy; }

    // casting
    operator float* () { return (float *)this; }
    operator const float* () const { return (const float *)this; }

    // assignment operators
    D3DXVECTOR2& operator += (const D3DXVECTOR2& v) { x += v.x; y += v.y; return *this; }
    D3DXVECTOR2& operator -= (const D3DXVECTOR2& v) { x -= v.x; y -= v.y; return *this; }
    D3DXVECTOR2& operator *= (float f) { x *= f; y *= f; return *this; }
    D3DXVECTOR2& operator /= (float f) { x /= f; y /= f; return *this; }

    // unary operators
    D3DXVECTOR2 operator + () const { return *this; }
    D3DXVECTOR2 operator - () const { return D3DXVECTOR2(-x, -y); }

    // binary operators
    D3DXVECTOR2 operator + (const D3DXVECTOR2& v) const { return D3DXVECTOR2(x + v.x, y + v.y); }
    D3DXVECTOR2 operator - (const D3DXVECTOR2& v) const { return D3DXVECTOR2(x - v.x, y - v.y); }
    D3DXVECTOR2 operator * (float f) const { return D3DXVECTOR2(x * f, y * f); }
    D3DXVECTOR2 operator / (float f) const { return D3DXVECTOR2(x / f, y / f); }

    friend D3DXVECTOR2 operator * (float f, const struct D3DXVECTOR2& v) { return D3DXVECTOR2(f * v.x, f * v.y); }

    BOOL operator == (const D3DXVECTOR2& v) const { return x == v.x && y == v.y; }
    BOOL operator != (const D3DXVECTOR2& v) const { return x != v.x || y != v.y; }

} D3DXVECTOR2, *LPD3DXVECTOR2;

typedef struct D3DXVECTOR3 {
    float x, y, z;

    D3DXVECTOR3() {}
    D3DXVECTOR3(const float *pf) { x = pf[0]; y = pf[1]; z = pf[2]; }
    D3DXVECTOR3(const D3DXVECTOR2& v, float fz) { x = v.x; y = v.y; z = fz; }
    D3DXVECTOR3(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }

    operator float* () { return (float *)this; }
    operator const float* () const { return (const float *)this; }

    D3DXVECTOR3& operator += (const D3DXVECTOR3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    D3DXVECTOR3& operator -= (const D3DXVECTOR3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    D3DXVECTOR3& operator *= (float f) { x *= f; y *= f; z *= f; return *this; }
    D3DXVECTOR3& operator /= (float f) { x /= f; y /= f; z /= f; return *this; }

    D3DXVECTOR3 operator + () const { return *this; }
    D3DXVECTOR3 operator - () const { return D3DXVECTOR3(-x, -y, -z); }

    D3DXVECTOR3 operator + (const D3DXVECTOR3& v) const { return D3DXVECTOR3(x + v.x, y + v.y, z + v.z); }
    D3DXVECTOR3 operator - (const D3DXVECTOR3& v) const { return D3DXVECTOR3(x - v.x, y - v.y, z - v.z); }
    D3DXVECTOR3 operator * (float f) const { return D3DXVECTOR3(x * f, y * f, z * f); }
    D3DXVECTOR3 operator / (float f) const { return D3DXVECTOR3(x / f, y / f, z / f); }

    friend D3DXVECTOR3 operator * (float f, const struct D3DXVECTOR3& v) { return D3DXVECTOR3(f * v.x, f * v.y, f * v.z); }

    BOOL operator == (const D3DXVECTOR3& v) const { return x == v.x && y == v.y && z == v.z; }
    BOOL operator != (const D3DXVECTOR3& v) const { return x != v.x || y != v.y || z != v.z; }

} D3DXVECTOR3, *LPD3DXVECTOR3;

typedef struct D3DXVECTOR4 {
    float x, y, z, w;

    D3DXVECTOR4() {}
    D3DXVECTOR4(const float *pf) { x = pf[0]; y = pf[1]; z = pf[2]; w = pf[3]; }
    D3DXVECTOR4(const D3DXVECTOR3& v, float fw) { x = v.x; y = v.y; z = v.z; w = fw; }
    D3DXVECTOR4(float fx, float fy, float fz, float fw) { x = fx; y = fy; z = fz; w = fw; }

    operator float* () { return (float *)this; }
    operator const float* () const { return (const float *)this; }

    D3DXVECTOR4& operator += (const D3DXVECTOR4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    D3DXVECTOR4& operator -= (const D3DXVECTOR4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    D3DXVECTOR4& operator *= (float f) { x *= f; y *= f; z *= f; w *= f; return *this; }
    D3DXVECTOR4& operator /= (float f) { x /= f; y /= f; z /= f; w /= f; return *this; }

    D3DXVECTOR4 operator + () const { return *this; }
    D3DXVECTOR4 operator - () const { return D3DXVECTOR4(-x, -y, -z, -w); }

    D3DXVECTOR4 operator + (const D3DXVECTOR4& v) const { return D3DXVECTOR4(x + v.x, y + v.y, z + v.z, w + v.w); }
    D3DXVECTOR4 operator - (const D3DXVECTOR4& v) const { return D3DXVECTOR4(x - v.x, y - v.y, z - v.z, w - v.w); }
    D3DXVECTOR4 operator * (float f) const { return D3DXVECTOR4(x * f, y * f, z * f, w * f); }
    D3DXVECTOR4 operator / (float f) const { return D3DXVECTOR4(x / f, y / f, z / f, w / f); }

    friend D3DXVECTOR4 operator * (float f, const struct D3DXVECTOR4& v) { return D3DXVECTOR4(f * v.x, f * v.y, f * v.z, f * v.w); }

    BOOL operator == (const D3DXVECTOR4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
    BOOL operator != (const D3DXVECTOR4& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }

} D3DXVECTOR4, *LPD3DXVECTOR4;

typedef struct D3DXMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };

    D3DXMATRIX() {}
    D3DXMATRIX(const float *pf) { memcpy(&_11, pf, sizeof(D3DXMATRIX)); }
    D3DXMATRIX(float f11, float f12, float f13, float f14,
               float f21, float f22, float f23, float f24,
               float f31, float f32, float f33, float f34,
               float f41, float f42, float f43, float f44)
    {
        _11 = f11; _12 = f12; _13 = f13; _14 = f14;
        _21 = f21; _22 = f22; _23 = f23; _24 = f24;
        _31 = f31; _32 = f32; _33 = f33; _34 = f34;
        _41 = f41; _42 = f42; _43 = f43; _44 = f44;
    }

    operator float* () { return (float *)this; }
    operator const float* () const { return (const float *)this; }

    D3DXMATRIX& operator *= (const D3DXMATRIX& mat);
    D3DXMATRIX& operator += (const D3DXMATRIX& mat);
    D3DXMATRIX& operator -= (const D3DXMATRIX& mat);
    D3DXMATRIX& operator *= (float f);
    D3DXMATRIX& operator /= (float f);

    D3DXMATRIX operator + () const { return *this; }
    D3DXMATRIX operator - () const { return D3DXMATRIX(-_11, -_12, -_13, -_14, -_21, -_22, -_23, -_24, -_31, -_32, -_33, -_34, -_41, -_42, -_43, -_44); }

    D3DXMATRIX operator * (const D3DXMATRIX& mat) const;
    D3DXMATRIX operator + (const D3DXMATRIX& mat) const;
    D3DXMATRIX operator - (const D3DXMATRIX& mat) const;
    D3DXMATRIX operator * (float f) const;
    D3DXMATRIX operator / (float f) const;

    BOOL operator == (const D3DXMATRIX& mat) const { return memcmp(this, &mat, sizeof(D3DXMATRIX)) == 0; }
    BOOL operator != (const D3DXMATRIX& mat) const { return memcmp(this, &mat, sizeof(D3DXMATRIX)) != 0; }

} D3DXMATRIX, *LPD3DXMATRIX;

typedef struct D3DXQUATERNION {
    float x, y, z, w;
    D3DXQUATERNION() {}
    D3DXQUATERNION(const float *pf) { x = pf[0]; y = pf[1]; z = pf[2]; w = pf[3]; }
    D3DXQUATERNION(float fx, float fy, float fz, float fw) { x = fx; y = fy; z = fz; w = fw; }
    operator float* () { return (float *)this; }
    operator const float* () const { return (const float *)this; }
} D3DXQUATERNION, *LPD3DXQUATERNION;

typedef struct D3DXPLANE {
    float a, b, c, d;
    D3DXPLANE() {}
    D3DXPLANE(const float *pf) { a = pf[0]; b = pf[1]; c = pf[2]; d = pf[3]; }
    D3DXPLANE(float fa, float fb, float fc, float fd) { a = fa; b = fb; c = fc; d = fd; }
    operator float* () { return (float *)this; }
    operator const float* () const { return (const float *)this; }
} D3DXPLANE, *LPD3DXPLANE;

// Function declarations
D3DXVECTOR3* WINAPI D3DXVec3Normalize(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV);
float WINAPI D3DXVec3Length(const D3DXVECTOR3* pV);
D3DXVECTOR3* WINAPI D3DXVec3Cross(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2);
float WINAPI D3DXVec3Dot(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2);

D3DXMATRIX* WINAPI D3DXMatrixIdentity(D3DXMATRIX* pOut);
D3DXMATRIX* WINAPI D3DXMatrixTranspose(D3DXMATRIX* pOut, const D3DXMATRIX* pM);
D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2);
D3DXMATRIX* WINAPI D3DXMatrixInverse(D3DXMATRIX* pOut, float* pDeterminant, const D3DXMATRIX* pM);

D3DXMATRIX* WINAPI D3DXMatrixLookAtLH(D3DXMATRIX* pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp);
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pOut, float fovy, float Aspect, float zn, float zf);
D3DXMATRIX* WINAPI D3DXMatrixOrthoLH(D3DXMATRIX* pOut, float w, float h, float zn, float zf);
D3DXMATRIX* WINAPI D3DXMatrixRotationX(D3DXMATRIX* pOut, float angle);
D3DXMATRIX* WINAPI D3DXMatrixRotationY(D3DXMATRIX* pOut, float angle);
D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX* pOut, float angle);
D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX* pOut, float x, float y, float z);
D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX* pOut, float sx, float sy, float sz);

D3DXQUATERNION* WINAPI D3DXQuaternionRotationMatrix(D3DXQUATERNION* pOut, const D3DXMATRIX* pM);
D3DXQUATERNION* WINAPI D3DXQuaternionSlerp(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float t);

// Add more as needed by the codebase
