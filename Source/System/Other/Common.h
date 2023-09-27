#pragma once

constexpr uint32_t SCREEN_WIDTH{ 1280 };
constexpr uint32_t SCREEN_HEIGHT{ 720 };

constexpr float PI = 3.14159265f;

using namespace DirectX;

//-----< XMFloat2 >-----//
// ‘«‚µŽZ(XMFLOAT2+XMFLOAT2)
inline XMFLOAT2 operator+ (const XMFLOAT2 a, const XMFLOAT2 b)
{
    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A + B;
    XMFLOAT2 ans{};

    XMStoreFloat2(&ans, Ans);
    return ans;
}
inline void operator+= (XMFLOAT2& a, const XMFLOAT2 b)
{
    a.x += b.x;
    a.y += b.y;
}
// ˆø‚«ŽZ(XMFLOAT2-XMFLOAT2)
inline XMFLOAT2 operator- (const XMFLOAT2 a, const XMFLOAT2 b)
{
    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A - B;
    XMFLOAT2 ans;

    XMStoreFloat2(&ans, Ans);
    return ans;
}
inline void operator-= (XMFLOAT2& a, const XMFLOAT2 b)
{
    a.x -= b.x;
    a.y -= b.y;
}
// Š|‚¯ŽZ(XMFLOAT2*XMFLOAT2)
inline XMFLOAT2 operator* (const XMFLOAT2 a, const XMFLOAT2 b)
{
    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A * B;
    XMFLOAT2 ans;

    XMStoreFloat2(&ans, Ans);
    return ans;
}
inline void operator*= (XMFLOAT2& a, const XMFLOAT2 b)
{
    a.x *= b.x;
    a.y *= b.y;
}
// Š„‚èŽZ(XMFLOAT2/XMFLOAT2)
inline XMFLOAT2 operator/ (const XMFLOAT2 a, const XMFLOAT2 b)
{
    const XMVECTOR A = XMLoadFloat2(&a);
    const XMVECTOR B = XMLoadFloat2(&b);
    const XMVECTOR Ans = A / B;
    XMFLOAT2 ans;

    XMStoreFloat2(&ans, Ans);
    return ans;
}
inline void operator/= (XMFLOAT2& a, const XMFLOAT2 b)
{
    a.x /= b.x;
    a.y /= b.y;
}
// ‘«‚µŽZ(XMFLOAT2+float)
inline XMFLOAT2 operator+ (const XMFLOAT2 a, const float b)
{
    const XMFLOAT2 ans =
    {
        a.x + b,
        a.y + b
    };

    return ans;
}
inline void operator+= (XMFLOAT2& a, const float b)
{
    a.x += b;
    a.y += b;
}
// ˆø‚«ŽZ(XMFLOAT2-float)
inline XMFLOAT2 operator- (const XMFLOAT2 a, const float b)
{
    const XMFLOAT2 ans =
    {
        a.x - b,
        a.y - b
    };

    return ans;
}
inline void operator-= (XMFLOAT2& a, const float b)
{
    a.x -= b;
    a.y -= b;
}
// Š|‚¯ŽZ(XMFLOAT2*float)
inline XMFLOAT2 operator* (const XMFLOAT2 a, const float b)
{
    const XMFLOAT2 ans =
    {
        a.x * b,
        a.y * b
    };

    return ans;
}
inline void operator*= (XMFLOAT2& a, const float b)
{
    a.x *= b;
    a.y *= b;
}
// Š„‚èŽZ(XMFLOAT2/float)
inline XMFLOAT2 operator/ (const XMFLOAT2 a, const float b)
{
    const XMFLOAT2 ans =
    {
        a.x / b,
        a.y / b
    };

    return ans;
}
inline void operator/= (XMFLOAT2& a, const float b)
{
    a.x /= b;
    a.y /= b;
}



//-----< XMFLOAT3 >-----//
// ‘«‚µŽZ(XMFLOAT3+XMFLOAT3)
inline XMFLOAT3 operator+ (const XMFLOAT3 a, const XMFLOAT3 b)
{
    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A + B;
    XMFLOAT3 ans{};

    XMStoreFloat3(&ans, Ans);
    return ans;
}
inline void operator+= (XMFLOAT3& a, const XMFLOAT3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}
// ˆø‚«ŽZ(XMFLOAT3-XMFLOAT3)
inline XMFLOAT3 operator- (const XMFLOAT3 a, const XMFLOAT3 b)
{
    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A - B;
    XMFLOAT3 ans;

    XMStoreFloat3(&ans, Ans);
    return ans;
}
inline void operator-= (XMFLOAT3& a, const XMFLOAT3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}
// Š|‚¯ŽZ(XMFLOAT3*XMFLOAT3)
inline XMFLOAT3 operator* (const XMFLOAT3 a, const XMFLOAT3 b)
{
    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A * B;
    XMFLOAT3 ans;

    XMStoreFloat3(&ans, Ans);
    return ans;
}
inline void operator*= (XMFLOAT3& a, const XMFLOAT3 b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
}
// Š„‚èŽZ(XMFLOAT3/XMFLOAT3)
inline XMFLOAT3 operator/ (const XMFLOAT3 a, const XMFLOAT3 b)
{
    const XMVECTOR A = XMLoadFloat3(&a);
    const XMVECTOR B = XMLoadFloat3(&b);
    const XMVECTOR Ans = A / B;
    XMFLOAT3 ans;

    XMStoreFloat3(&ans, Ans);
    return ans;
}
inline void operator/= (XMFLOAT3& a, const XMFLOAT3 b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
}
// ‘«‚µŽZ(XMFLOAT3+float)
inline XMFLOAT3 operator+ (const XMFLOAT3 a, const float b)
{
    const XMFLOAT3 ans =
    {
        a.x + b,
        a.y + b,
        a.z + b,
    };

    return ans;
}
inline void operator+= (XMFLOAT3& a, const float b)
{
    a.x += b;
    a.y += b;
    a.z += b;
}
// ˆø‚«ŽZ(XMFLOAT3-float)
inline XMFLOAT3 operator- (const XMFLOAT3 a, const float b)
{
    const XMFLOAT3 ans =
    {
        a.x - b,
        a.y - b,
        a.z - b
    };

    return ans;
}
inline void operator-= (XMFLOAT3& a, const float b)
{
    a.x -= b;
    a.y -= b;
    a.z -= b;
}
// Š|‚¯ŽZ(XMFLOAT3*float)
inline XMFLOAT3 operator* (const XMFLOAT3 a, const float b)
{
    const XMFLOAT3 ans =
    {
        a.x * b,
        a.y * b,
        a.z * b,
    };

    return ans;
}
inline void operator*= (XMFLOAT3& a, const float b)
{
    a.x *= b;
    a.y *= b;
    a.z *= b;
}
// Š„‚èŽZ(XMFLOAT3/float)
inline XMFLOAT3 operator/ (const XMFLOAT3 a, const float b)
{
    const XMFLOAT3 ans =
    {
        a.x / b,
        a.y / b,
        a.z / b,
    };

    return ans;
}
inline void operator/= (XMFLOAT3& a, const float b)
{
    a.x /= b;
    a.y /= b;
    a.z /= b;
}



//-----< XMFLOAT4 >-----//
// ‘«‚µŽZ(XMFLOAT4+XMFLOAT4)
inline XMFLOAT4 operator+ (const XMFLOAT4 a, const XMFLOAT4 b)
{
    const XMVECTOR A = XMLoadFloat4(&a);
    const XMVECTOR B = XMLoadFloat4(&b);
    const XMVECTOR Ans = A + B;
    XMFLOAT4 ans{};

    XMStoreFloat4(&ans, Ans);
    return ans;
}
inline void operator+= (XMFLOAT4& a, const XMFLOAT4 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
}
// ˆø‚«ŽZ(XMFLOAT4-XMFLOAT4)
inline XMFLOAT4 operator- (const XMFLOAT4 a, const XMFLOAT4 b)
{
    const XMVECTOR A = XMLoadFloat4(&a);
    const XMVECTOR B = XMLoadFloat4(&b);
    const XMVECTOR Ans = A - B;
    XMFLOAT4 ans;

    XMStoreFloat4(&ans, Ans);
    return ans;
}
inline void operator-= (XMFLOAT4& a, const XMFLOAT4 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    a.w -= b.w;
}
// Š|‚¯ŽZ(XMFLOAT4*XMFLOAT4)
inline XMFLOAT4 operator* (const XMFLOAT4 a, const XMFLOAT4 b)
{
    const XMVECTOR A = XMLoadFloat4(&a);
    const XMVECTOR B = XMLoadFloat4(&b);
    const XMVECTOR Ans = A * B;
    XMFLOAT4 ans;

    XMStoreFloat4(&ans, Ans);
    return ans;
}
inline void operator*= (XMFLOAT4& a, const XMFLOAT4 b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    a.w *= b.w;
}
// Š„‚èŽZ(XMFLOAT4/XMFLOAT4)
inline XMFLOAT4 operator/ (const XMFLOAT4 a, const XMFLOAT4 b)
{
    const XMVECTOR A = XMLoadFloat4(&a);
    const XMVECTOR B = XMLoadFloat4(&b);
    const XMVECTOR Ans = A / B;
    XMFLOAT4 ans;

    XMStoreFloat4(&ans, Ans);
    return ans;
}
inline void operator/= (XMFLOAT4& a, const XMFLOAT4 b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
    a.w /= b.w;
}
// ‘«‚µŽZ(XMFLOAT4+float)
inline XMFLOAT4 operator+ (const XMFLOAT4 a, const float b)
{
    const XMFLOAT4 ans =
    {
        a.x + b,
        a.y + b,
        a.z + b,
        a.w + b
    };

    return ans;
}
inline void operator+= (XMFLOAT4& a, const float b)
{
    a.x += b;
    a.y += b;
    a.z += b;
    a.w += b;
}
// ˆø‚«ŽZ(XMFLOAT4-float)
inline XMFLOAT4 operator- (const XMFLOAT4 a, const float b)
{
    const XMFLOAT4 ans =
    {
        a.x - b,
        a.y - b,
        a.z - b,
        a.w - b
    };

    return ans;
}
inline void operator-= (XMFLOAT4& a, const float b)
{
    a.x -= b;
    a.y -= b;
    a.z -= b;
    a.w -= b;
}
// Š|‚¯ŽZ(XMFLOAT4*float)
inline XMFLOAT4 operator* (const XMFLOAT4 a, const float b)
{
    const XMFLOAT4 ans =
    {
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b,
    };

    return ans;
}
inline void operator*= (XMFLOAT4& a, const float b)
{
    a.x *= b;
    a.y *= b;
    a.z *= b;
    a.w *= b;
}
// Š„‚èŽZ(XMFLOAT4/float)
inline XMFLOAT4 operator/ (const XMFLOAT4 a, const float b)
{
    const XMFLOAT4 ans =
    {
        a.x / b,
        a.y / b,
        a.z / b,
        a.w / b,
    };

    return ans;
}
inline void operator/= (XMFLOAT4& a, const float b)
{
    a.x /= b;
    a.y /= b;
    a.z /= b;
    a.w /= b;
}

//-----< XMFLOAT4X4 >-----//
// Š|‚¯ŽZ(XMFLOAT4X4*XMFLOAT4X4)
inline XMFLOAT4X4 operator* (const XMFLOAT4X4 a, const XMFLOAT4X4 b)
{
    XMMATRIX A = XMLoadFloat4x4(&a);
    XMMATRIX B = XMLoadFloat4x4(&b);

    XMFLOAT4X4 ans;
    XMStoreFloat4x4(&ans, A * B);

    return ans;
}