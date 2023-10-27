#pragma once
#include<DirectXMath.h>
#include <algorithm>
#include<vector>

#include "Common.h"

//-----< 実体を削除する関数 >-----//
template <typename T>
inline void SafeDelete(T*& p)
{
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}

//-----< 最大値・最小値に収める関数 >-----//
inline float Clamp(float value, const float min, const float max)
{
    assert(max >= min);
    return value > max ? max : value < min ? min : value;
}

//-----< Radian値の補正 >-----//
inline float CirculateRadian(float radian)
{
    constexpr float maxRad = DirectX::XMConvertToRadians(180);
    constexpr float minRad = DirectX::XMConvertToRadians(-180);

    if (radian < minRad)
    {
        radian += maxRad * 2.0f;
    }
    if (radian > maxRad)
    {
        radian += minRad * 2.0f;
    }

    return radian;
}

//-----< Degree値の補正 >-----//
inline float CirculateDegree(float degree)
{
    if (degree < -180.0f)
    {
        degree += 360.0f;
    }
    if (degree > 180.0f)
    {
        degree -= 360.0f;
    }

    return degree;
}

//オイラー角からクオータニオンの変換
inline XMFLOAT4 EulerToQuaternion(XMFLOAT3 radian)
{
    XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(radian.x, radian.y, radian.z);
    XMFLOAT4 q{};
    XMStoreFloat4(&q, Q);

    return q;
}

//クオータニオンからオイラー角への変換
inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 quaternion)
{
    const float xx = quaternion.x * quaternion.x;
    const float yy = quaternion.y * quaternion.y;
    const float zz = quaternion.z * quaternion.z;

    const float m31 = 2.f * quaternion.x * quaternion.z + 2.f * quaternion.y * quaternion.w;
    const float m32 = 2.f * quaternion.y * quaternion.z - 2.f * quaternion.x * quaternion.w;
    const float m33 = 1.f - 2.f * xx - 2.f * yy;

    const float cy = sqrtf(m33 * m33 + m31 * m31);
    const float cx = atan2f(-m32, cy);
    if (cy > 16.f * FLT_EPSILON)
    {
        const float m12 = 2.f * quaternion.x * quaternion.y + 2.f * quaternion.z * quaternion.w;
        const float m22 = 1.f - 2.f * xx - 2.f * zz;

        return XMFLOAT3(cx, atan2f(m31, m33), atan2f(m12, m22));
    }
    else
    {
        const float m11 = 1.f - 2.f * yy - 2.f * zz;
        const float m21 = 2.f * quaternion.x * quaternion.y - 2.f * quaternion.z * quaternion.w;

        return XMFLOAT3(cx, 0.f, atan2f(-m21, m11));
    }
}

//-----< A地点からB地点へのベクトルを求める関数 >-----//
inline XMFLOAT2 MakeVectorAtoB(const XMFLOAT2& a, const XMFLOAT2& b, bool isNormalized = false)
{
    XMVECTOR A = XMLoadFloat2(&a);
    XMVECTOR B = XMLoadFloat2(&b);
    XMVECTOR AtoB = B - A;

    if (!isNormalized)AtoB = XMVector3Normalize(AtoB);

    XMFLOAT2 atob = { 0,0 };
    XMStoreFloat2(&atob, AtoB);
    return atob;
}
inline XMFLOAT3 MakeVectorAtoB(const XMFLOAT3& a, const XMFLOAT3& b, bool isNormalized = false)
{
    XMVECTOR A = XMLoadFloat3(&a);
    XMVECTOR B = XMLoadFloat3(&b);
    XMVECTOR AtoB = B - A;

    if (!isNormalized)AtoB = XMVector3Normalize(AtoB);

    XMFLOAT3 atob = { 0,0,0 };
    XMStoreFloat3(&atob, AtoB);
    return atob;
}

//-----< ベクトルの長さを求める関数 >-----//
inline float LengthFloat2(XMFLOAT2 vec)
{
    XMVECTOR Vec = XMLoadFloat2(&vec);

    XMVECTOR Length = XMVector2Length(Vec);
    float length = 0.0f;
	XMStoreFloat(&length, Length);
    return length;
}
inline float LengthFloat3(XMFLOAT3 vec)
{
    XMVECTOR Vec = XMLoadFloat3(&vec);

    XMVECTOR Length = XMVector3Length(Vec);
    float length = 0.0f;
    XMStoreFloat(&length, Length);
    return length;
}

//-----< 正規化 >-----//
inline XMFLOAT2 NormalizeFloat2(XMFLOAT2 vec)
{
    XMVECTOR V = XMLoadFloat2(&vec);
    V = XMVector2Normalize(V);
    XMFLOAT2 v{};
    XMStoreFloat2(&v, V);
    return v;
}
inline XMFLOAT3 NormalizeFloat3(XMFLOAT3 vec)
{
    XMVECTOR V = XMLoadFloat3(&vec);
    V = XMVector3Normalize(V);
    XMFLOAT3 v{};
    XMStoreFloat3(&v, V);
    return v;
}
inline XMFLOAT4 NormalizeFloat4(XMFLOAT4 vec)
{
    XMVECTOR V = XMLoadFloat4(&vec);
    V = XMVector4Normalize(V);
    XMFLOAT4 v{};
    XMStoreFloat4(&v, V);
    return v;
}

//絶対値を返す
inline XMFLOAT2 AbsoluteFloat2(XMFLOAT2 vec)
{
    XMFLOAT2 v = { fabsf(vec.x),fabsf(vec.y) };
    return v;
}
inline XMFLOAT3 AbsoluteFloat3(XMFLOAT3 vec)
{
    XMFLOAT3 v = { fabsf(vec.x),fabsf(vec.y) ,fabsf(vec.z) };
    return v;
}

//-----< 出力に文字を出力する >-----//
inline void OutputDebugLog(std::string str)
{
    std::stringstream debugStr;
    debugStr << str;
    OutputDebugStringA(debugStr.str().c_str());
}
inline void OutputDebugLog(std::string str, DirectX::XMFLOAT3 val)
{
    str = str + " = { " + std::to_string(val.x) + ", " + std::to_string(val.y) + ", " + std::to_string(val.z) + " }\n";
    std::stringstream debugStr;
    debugStr << str;
    OutputDebugStringA(debugStr.str().c_str());
}

//-----< 補間 >-----//
#undef min
#undef max
//線形補間
inline float LerpFloat(float begin, float end, float ratio)
{
    if (ratio <= 0.0f) return begin;
    if (ratio >= 1.0f) return end;

	ratio = std::min(1.0f, std::max(ratio, 0.0f)); // 補完率を0.0f~1.0fの間に矯正
    return (begin * (1.0f - ratio)) + (end * ratio);
}
inline XMFLOAT2 LerpFloat2(XMFLOAT2 begin, XMFLOAT2 end, float ratio)
{
    if (ratio <= 0.0f) return begin;
    if (ratio >= 1.0f) return end;

    XMVECTOR Begin = XMLoadFloat2(&begin);
    XMVECTOR End = XMLoadFloat2(&end);

    XMVECTOR Ans = XMVectorLerp(Begin, End, ratio);
    XMFLOAT2 ans{};
    XMStoreFloat2(&ans, Ans);
    return ans;
}
inline XMFLOAT3 LerpFloat3(XMFLOAT3 begin, XMFLOAT3 end, float ratio)
{
    if (ratio <= 0.0f) return begin;
    if (ratio >= 1.0f) return end;

    XMVECTOR Begin = XMLoadFloat3(&begin);
    XMVECTOR End = XMLoadFloat3(&end);

    XMVECTOR Ans = XMVectorLerp(Begin, End, ratio);
    XMFLOAT3 ans{};
    XMStoreFloat3(&ans, Ans);
    return ans;
}
inline XMFLOAT4 LerpFloat4(XMFLOAT4 begin, XMFLOAT4 end, float ratio)
{
    if (ratio <= 0.0f) return begin;
    if (ratio >= 1.0f) return end;

    XMVECTOR Begin = XMLoadFloat4(&begin);
    XMVECTOR End = XMLoadFloat4(&end);

    XMVECTOR Ans = XMVectorLerp(Begin, End, ratio);
    XMFLOAT4 ans{};
    XMStoreFloat4(&ans, Ans);
    return ans;
}
//球面線形補間（原点を中心に回転）
inline XMFLOAT3 SlerpFloat3(XMFLOAT3 begin, XMFLOAT3 end, float ratio)
{
    //正規化
    XMFLOAT3 nBegin = NormalizeFloat3(begin);
    XMFLOAT3 nEnd = NormalizeFloat3(end);

    //2つのベクトルのなす角を求める
    const XMVECTOR Begin = XMLoadFloat3(&nBegin);
    const XMVECTOR End = XMLoadFloat3(&nEnd);
    const XMVECTOR Dot = XMVector3Dot(Begin, End);
    float dot{};
    XMStoreFloat(&dot, Dot);
    float rad = acos(dot);

     if (rad > XMConvertToRadians(5.0f))
    {
        XMFLOAT3 magnitudeLerp = LerpFloat3(nBegin, nEnd, ratio);
        XMFLOAT3 slerpVector = (nBegin * sinf((1.0f - ratio) * rad) + nEnd * sinf(ratio * rad)) / sinf(rad);

        return magnitudeLerp * slerpVector;
    }
    else
    {
	    return LerpFloat3(nBegin, nEnd, ratio);
    }
}
//ベジェ曲線
inline XMFLOAT2 BezierFloat2(std::vector<XMFLOAT2> controllPoints, float ratio)
{
    const size_t size = controllPoints.size();

    if (size == 0)assert("Not ControllPoint");
    if (size == 1)return controllPoints.at(0);

    float x = 0.0f;
    float y = 0.0f;

    auto Combination = [](int n, int k)
    {
        int up = 1;
        int down = 1;

        for (int i = 0; i < k; i++)
        {
            up *= (n - i);
            down *= (k - i);
        }

        return (up / down);
    };

    for (int i = 0; i < size; i++)
    {
        float coefficient = static_cast<float>(Combination(static_cast<int>(size) - 1, static_cast<int>(i)));
        coefficient *= (powf((1.0f - ratio), static_cast<float>(size - 1 - i)));
        coefficient *= (powf(ratio, static_cast<float>(i)));

        x += coefficient * controllPoints.at(i).x;
        y += coefficient * controllPoints.at(i).y;
    }

    return { x,y };
}
inline XMFLOAT3 BezierFloat3(std::vector<XMFLOAT3> controllPoints, float ratio)
{
    const size_t size = controllPoints.size();

    if (size == 0)assert("Not ControllPoint");
    if (size == 1)return controllPoints.at(0);

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    auto Combination = [](int n, int k)
    {
        int up = 1;
        int down = 1;

        for (int i = 0; i < k; i++)
        {
            up *= (n - i);
            down *= (k - i);
        }

        return (up / down);
    };

    for (int i = 0; i < size; i++)
    {
        float coefficient = static_cast<float>(Combination(static_cast<int>(size) - 1, static_cast<int>(i)));
        coefficient *= (powf((1.0f - ratio), static_cast<float>(size - 1 - i)));
        coefficient *= (powf(ratio, static_cast<float>(i)));

        x += coefficient * controllPoints.at(i).x;
        y += coefficient * controllPoints.at(i).y;
        z += coefficient * controllPoints.at(i).z;
    }

    return { x,y,z };
}
//エルミート曲線
inline XMFLOAT2 HermiteFloat2(std::vector<XMFLOAT2> controllPoints, float ratio)
{
	const size_t size = controllPoints.size();

	if (size == 0)assert("Not ControllPoint");
	if (size == 1)return controllPoints.at(0);

	//-----< 今どこの区間にいるか >-----//
	std::vector<float> sectionLength;
	float length = 0.0f;
	//各区間の長さを求める
	for (size_t i = 0; i < size - 1; i++)
	{
		const XMFLOAT2 v = {
	controllPoints.at(i + 1).x - controllPoints.at(i).x,
	controllPoints.at(i + 1).y - controllPoints.at(i).y };

		const float l = sqrtf(v.x * v.x + v.y * v.y);

		sectionLength.emplace_back(l);
		length += l;
	}

	std::vector<float> sections;
	//各区間の割合を求める
	for (int i = 0; i < size - 1; i++)
	{
		sections.emplace_back(sectionLength.at(i) / length);
	}

	size_t sectionNum = 0;
	float r = 0.0f;
	for (sectionNum; sectionNum < sections.size(); sectionNum++)
	{
		r += sections.at(sectionNum);
		if ((ratio - r) <= 0.00001f)
		{
			break;
		}
	}

	XMFLOAT2 dummy = {
		2.0f * controllPoints.at(0).x - controllPoints.at(1).x,
		2.0f * controllPoints.at(0).y - controllPoints.at(1).y,
	};
	const auto it = controllPoints.begin();
	controllPoints.insert(it, dummy);
	dummy = {
	2.0f * controllPoints.at(size - 1).x - controllPoints.at(size - 2).x,
	2.0f * controllPoints.at(size - 1).y - controllPoints.at(size - 2).y,
	};
	controllPoints.emplace_back(dummy);

	sectionNum++;
	float sectionRatio = (ratio - (r - sections.at(sectionNum - 1))) / sections.at(sectionNum - 1);

	XMVECTOR Out;

	const float power = 0.5f; // Usually power is 0.5f
	XMVECTOR P0 = XMLoadFloat2(&controllPoints.at(sectionNum - 1));
	XMVECTOR P1 = XMLoadFloat2(&controllPoints.at(sectionNum + 0));
	XMVECTOR P2 = XMLoadFloat2(&controllPoints.at(sectionNum + 1));
	XMVECTOR P3 = XMLoadFloat2(&controllPoints.at(sectionNum + 2));
	XMVECTOR V0 = XMVectorScale(XMVectorSubtract(P2, P0), power);
	XMVECTOR V1 = XMVectorScale(XMVectorSubtract(P3, P1), power);

	Out = powf(sectionRatio, 3.0f) * (2.0f * P1 - 2.0f * P2 + V0 + V1);
	Out += powf(sectionRatio, 2.0f) * (-3.0f * P1 + 3.0f * P2 - 2.0f * V0 - V1);
	Out += sectionRatio * V0 + P1;


	XMFLOAT2 out{};
	XMStoreFloat2(&out, Out);

	return out;
}
inline XMFLOAT3 HermiteFloat3(std::vector<XMFLOAT3> controllPoints, float ratio)
{
	const size_t size = controllPoints.size();

	if (size == 0)assert("Not ControllPoint");
	if (size == 1)return controllPoints.at(0);

	//-----< 今どこの区間にいるか >-----//
	std::vector<float> sectionLength;
	float length = 0.0f;
	//各区間の長さを求める
	for (size_t i = 0; i < size - 1; i++)
	{
		const XMFLOAT3 v = {
	controllPoints.at(i + 1).x - controllPoints.at(i).x,
	controllPoints.at(i + 1).y - controllPoints.at(i).y,
	controllPoints.at(i + 1).z - controllPoints.at(i).z };
		const float l = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

		sectionLength.emplace_back(l);
		length += l;
	}
	std::vector<float> sections;
	//各区間の割合を求める
	for (int i = 0; i < size - 1; i++)
	{
		sections.emplace_back(sectionLength.at(i) / length);
	}

    size_t sectionNum = 0;
	float r = 0.0f;
	for (sectionNum; sectionNum < sections.size(); sectionNum++)
	{
		r += sections.at(sectionNum);
		if ((ratio - r) <= 0.00001f)
		{
			break;
		}
	}

	XMFLOAT3 dummy = {
		2.0f * controllPoints.at(0).x - controllPoints.at(1).x,
		2.0f * controllPoints.at(0).y - controllPoints.at(1).y,
		2.0f * controllPoints.at(0).z - controllPoints.at(1).z,
	};
	const auto it = controllPoints.begin();
	controllPoints.insert(it, dummy);
	dummy = {
	2.0f * controllPoints.at(size - 1).x - controllPoints.at(size - 2).x,
	2.0f * controllPoints.at(size - 1).y - controllPoints.at(size - 2).y,
	2.0f * controllPoints.at(size - 1).z - controllPoints.at(size - 2).z,
	};
	controllPoints.emplace_back(dummy);

	sectionNum++;
	float sectionRatio = (ratio - (r - sections.at(sectionNum - 1))) / sections.at(sectionNum - 1);

	XMVECTOR Out;

	const float power = 0.5f; // Usually power is 0.5f
	XMVECTOR P0 = XMLoadFloat3(&controllPoints.at(sectionNum - 1));
	XMVECTOR P1 = XMLoadFloat3(&controllPoints.at(sectionNum + 0));
	XMVECTOR P2 = XMLoadFloat3(&controllPoints.at(sectionNum + 1));
	XMVECTOR P3 = XMLoadFloat3(&controllPoints.at(sectionNum + 2));
	XMVECTOR V0 = XMVectorScale(XMVectorSubtract(P2, P0), power);
	XMVECTOR V1 = XMVectorScale(XMVectorSubtract(P3, P1), power);

	Out = powf(sectionRatio, 3.0f) * (2.0f * P1 - 2.0f * P2 + V0 + V1);
	Out += powf(sectionRatio, 2.0f) * (-3.0f * P1 + 3.0f * P2 - 2.0f * V0 - V1);
	Out += sectionRatio * V0 + P1;


	XMFLOAT3 out{};
	XMStoreFloat3(&out, Out);

	return out;
}

//-----< 角度の変換 >-----//
inline XMFLOAT3 ConvertDegreeToRadian(XMFLOAT3 degree)
{
    const XMFLOAT3 radian
    {
        XMConvertToRadians(degree.x),
        XMConvertToRadians(degree.y),
        XMConvertToRadians(degree.z)
    };

    return radian;
}
inline XMFLOAT3 ConvertRadianToDegree(XMFLOAT3 radian)
{
    const XMFLOAT3 degree
    {
        XMConvertToDegrees(radian.x),
        XMConvertToDegrees(radian.y),
        XMConvertToDegrees(radian.z)
    };

    return degree;
}

//-----< 内積 >-----//
inline float DotFloat2(const XMFLOAT2 a, const XMFLOAT2 b)
{
    const XMVECTOR V0 = XMLoadFloat2(&a);
    const XMVECTOR V1 = XMLoadFloat2(&b);
    const XMVECTOR Ans = XMVector2Dot(V0, V1);
    float ans{};
    XMStoreFloat(&ans, Ans);
    return ans;

}
inline float DotFloat3(const XMFLOAT3 a, const XMFLOAT3 b)
{
    const XMVECTOR V0 = XMLoadFloat3(&a);
    const XMVECTOR V1 = XMLoadFloat3(&b);
    const XMVECTOR Ans = XMVector3Dot(V0, V1);
    float ans{};
    XMStoreFloat(&ans, Ans);
    return ans;
}

//-----< 外積 >-----//
inline float CrossFloat2(const XMFLOAT2& a, const XMFLOAT2& b)
{
    const XMVECTOR V0 = XMLoadFloat2(&a);
    const XMVECTOR V1 = XMLoadFloat2(&b);

    const XMVECTOR Ans = XMVector2Cross(V0, V1);
    float ans = 0.0f;
    XMStoreFloat(&ans, Ans);
    return ans;
}
inline XMFLOAT3 CrossFloat3(const XMFLOAT3& a, const XMFLOAT3& b)
{
    const XMVECTOR V0 = XMLoadFloat3(&a);
    const XMVECTOR V1 = XMLoadFloat3(&b);

    const XMVECTOR Ans = XMVector3Cross(V0, V1);
    XMFLOAT3 ans{};
    XMStoreFloat3(&ans, Ans);
    return ans;
}

//-----< 数値を循環させる >-----//
inline float ClampRotateFloat(float temp, const float min, const float max)
{
    float ans = temp;
    if (ans < min) ans = max;
    if (ans > max) ans = min;

    return ans;
}

//****************************************************************
// 
// カメラに映っているかどうかの判定
//      ViewProjMatrix : ビュープロジェクション行列
//      Position : オブジェクト(AABB)の中心位置
//      Size : AABBの1辺の半分
// 
//****************************************************************
inline bool IsInFrustum(const DirectX::XMMATRIX ViewProjMatrix, const DirectX::XMFLOAT3 position, const DirectX::XMFLOAT3 size)
{
    //ビュープロジェクション行列の逆行列
    XMMATRIX InvViewProjMatrix = XMMatrixInverse(nullptr, ViewProjMatrix);

    //ビュープロジェクション内の頂点算出用位置ベクトル
    XMVECTOR Verts[8] =
    {
        // near plane corners
        { -1,  1, 0 },	// [0]:左上
        {  1,  1, 0 },	// [1]:右上
        {  1, -1, 0 },	// [2]:右下
        { -1, -1, 0 },	// [3]:左下

        // far plane corners.
        { -1,  1, 1 },	// [4]:左上
        {  1,  1, 1 },	// [5]:右上
        {  1, -1, 1 },	// [6]:右下
        { -1, -1, 1 }	// [7]:左下
    };

    //ビュープロジェクション行列の逆行列を用いて、各頂点を算出する
    for (int i = 0; i < 8; i++)
    {
        Verts[i] = XMVector3TransformCoord(Verts[i], InvViewProjMatrix);
    }

    //視錐台（フラスタム）を構成する６平面を算出する
    // 0:左側面, 1:右側面, 2:下側面, 3:上側面, 4:奥側面, 5:手前側面
    // 全ての面の法線は内側を向くように設定する

    DirectX::XMFLOAT4X4 viewProjMatrix = {};
    DirectX::XMStoreFloat4x4(&viewProjMatrix, ViewProjMatrix);

    XMFLOAT3 normal[6]{};//６平面の法線
    float distance[6]{};//６平面の原点との最短距離

    // 左側面
    int index = 0;
    {
        //法線を求める
        normal[index] = {
        viewProjMatrix._14 + viewProjMatrix._11,
        viewProjMatrix._24 + viewProjMatrix._21,
        viewProjMatrix._34 + viewProjMatrix._31
        };
        XMStoreFloat3(&normal[index], XMVector3Normalize(XMLoadFloat3(&normal[index])));//正規化

        //原点との最短距離
        DirectX::XMFLOAT3 v{};
        DirectX::XMStoreFloat3(&v, Verts[3]);
        const float a = normal[index].x;
        const float b = normal[index].y;
        const float c = normal[index].z;
        const float d = normal[index].x * (-v.x) + normal[index].y * (-v.y) + normal[index].z * (-v.z);
        distance[index] = fabsf(d) / sqrtf(a * a + b * b + c * c);
    }
    // 右側面
    index = 1;
    {
        //法線を求める
        normal[index] = {
        viewProjMatrix._14 - viewProjMatrix._11,
        viewProjMatrix._24 - viewProjMatrix._21,
        viewProjMatrix._34 - viewProjMatrix._31
        };
        XMStoreFloat3(&normal[index], XMVector3Normalize(XMLoadFloat3(&normal[index])));//正規化

        //原点との最短距離
        DirectX::XMFLOAT3 v{};
        DirectX::XMStoreFloat3(&v, Verts[2]);
        const float a = normal[index].x;
        const float b = normal[index].y;
        const float c = normal[index].z;
        const float d = normal[index].x * (-v.x) + normal[index].y * (-v.y) + normal[index].z * (-v.z);
        distance[index] = fabsf(d) / sqrtf(a * a + b * b + c * c);
    }
    // 下側面
    index = 2;
    {
        //法線を求める
        normal[index] = {
        viewProjMatrix._14 + viewProjMatrix._12,
        viewProjMatrix._24 + viewProjMatrix._22,
        viewProjMatrix._34 + viewProjMatrix._32
        };
        XMStoreFloat3(&normal[index], XMVector3Normalize(XMLoadFloat3(&normal[index])));//正規化

        //原点との最短距離
        DirectX::XMFLOAT3 v{};
        DirectX::XMStoreFloat3(&v, Verts[3]);
        const float a = normal[index].x;
        const float b = normal[index].y;
        const float c = normal[index].z;
        const float d = normal[index].x * (-v.x) + normal[index].y * (-v.y) + normal[index].z * (-v.z);
        distance[index] = fabsf(d) / sqrtf(a * a + b * b + c * c);
    }
    // 上側面
    index = 3;
    {
        //法線を求める
        normal[index] = {
        viewProjMatrix._14 - viewProjMatrix._12,
        viewProjMatrix._24 - viewProjMatrix._22,
        viewProjMatrix._34 - viewProjMatrix._32
        };
        XMStoreFloat3(&normal[index], XMVector3Normalize(XMLoadFloat3(&normal[index])));//正規化

        //原点との最短距離
        DirectX::XMFLOAT3 v{};
        DirectX::XMStoreFloat3(&v, Verts[0]);
        const float a = normal[index].x;
        const float b = normal[index].y;
        const float c = normal[index].z;
        const float d = normal[index].x * (-v.x) + normal[index].y * (-v.y) + normal[index].z * (-v.z);
        distance[index] = fabsf(d) / sqrtf(a * a + b * b + c * c);
    }
    // 奥側面
    index = 4;
    {
        //法線を求める
        normal[index] = {
        -viewProjMatrix._14 - viewProjMatrix._13,
        -viewProjMatrix._24 - viewProjMatrix._23,
        -viewProjMatrix._34 - viewProjMatrix._33
        };
        XMStoreFloat3(&normal[index], XMVector3Normalize(XMLoadFloat3(&normal[index])));//正規化

        //原点との最短距離
        DirectX::XMFLOAT3 v{};
        DirectX::XMStoreFloat3(&v, Verts[4]);
        const float a = normal[index].x;
        const float b = normal[index].y;
        const float c = normal[index].z;
        const float d = normal[index].x * (-v.x) + normal[index].y * (-v.y) + normal[index].z * (-v.z);
        distance[index] = fabsf(d) / sqrtf(a * a + b * b + c * c);
    }
    // 奥側面
    index = 5;
    {
        //法線を求める
        normal[index] = {
        -viewProjMatrix._14 + viewProjMatrix._13,
        -viewProjMatrix._24 + viewProjMatrix._23,
        -viewProjMatrix._34 + viewProjMatrix._33
        };
        XMStoreFloat3(&normal[index], XMVector3Normalize(XMLoadFloat3(&normal[index])));//正規化

        //原点との最短距離
        DirectX::XMFLOAT3 v{};
        DirectX::XMStoreFloat3(&v, Verts[0]);
        const float a = normal[index].x;
        const float b = normal[index].y;
        const float c = normal[index].z;
        const float d = normal[index].x * (-v.x) + normal[index].y * (-v.y) + normal[index].z * (-v.z);
        distance[index] = fabsf(d) / sqrtf(a * a + b * b + c * c);
    }

    // いったん内部にあるとしておく
    bool crossing = false;//交差しているか

    for (int i = 0; i < 6; i++)
    {
        //各平面の法線の成分を用いてAABBの８頂点の中から最近点と最遠点を求める
        XMFLOAT3 nearPos = position;	// 最近点
        XMFLOAT3 farPos = position;	// 最遠点

        if (normal[i].x > 0.0f)
        {
            nearPos.x -= size.x;
            farPos.x += size.x;
        }
        else
        {
            nearPos.x += size.x;
            farPos.x -= size.x;
        }
        if (normal[i].y > 0.0f)
        {
            nearPos.y -= size.y;
            farPos.y += size.y;
        }
        else
        {
            nearPos.y += size.y;
            farPos.y -= size.y;
        }
        if (normal[i].z > 0.0f)
        {
            nearPos.z -= size.z;
            farPos.z += size.z;
        }
        else
        {
            nearPos.z += size.z;
            farPos.z -= size.z;
        }

        // 各平面との内積を計算し、交差・内外判定(表裏判定)を行う
        const DirectX::XMVECTOR NearPos = DirectX::XMLoadFloat3(&nearPos);
        const DirectX::XMVECTOR FarPos = DirectX::XMLoadFloat3(&farPos);
        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&normal[i]);
        //内積
        float nearDot = 0.0f, farDot = 0.0f;
        DirectX::XMStoreFloat(&nearDot, DirectX::XMVector3Dot(NearPos, Normal));
        DirectX::XMStoreFloat(&farDot, DirectX::XMVector3Dot(FarPos, Normal));
        //射影
        DirectX::XMFLOAT3 nearVec = { normal[i].x * nearDot,normal[i].y * nearDot ,normal[i].z * nearDot };
        DirectX::XMFLOAT3 farVec = { normal[i].x * farDot,normal[i].y * farDot ,normal[i].z * farDot };
        //射影の長さ
        float nearLength = sqrtf(nearVec.x * nearVec.x + nearVec.y * nearVec.y + nearVec.z * nearVec.z);
        float farLength = sqrtf(farVec.x * farVec.x + farVec.y * farVec.y + farVec.z * farVec.z);
        //内外判定
        if (nearLength > distance[i] && farLength > distance[i])
        {
            //外側
            return false;
        }
        if ((nearLength > distance[i] && farLength < distance[i]) || (nearLength < distance[i] && farLength > distance[i]))
        {
            //視錐台を構成する６平面のいずれかと交差している
            crossing = true;
        }
    }

    // AABBの面の中心点と法線を作り、中心点から視錐台の８頂点に対して表裏判定を行う。
    // ８頂点全てが表側にあれば、交差していなかったことが確定する
    if (crossing)
    {
        DirectX::XMFLOAT3 min = { position.x - size.x,position.y - size.y ,position.z - size.z };
        DirectX::XMFLOAT3 max = { position.x + size.x,position.y + size.y ,position.z + size.z };

        auto Judge = [](DirectX::XMFLOAT3 normal, DirectX::XMFLOAT3 center, const DirectX::XMVECTOR* Verts)
        {
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&normal);
            int judge = 0;
            for (int i = 0; i < 8; i++)
            {
                DirectX::XMVECTOR Center = DirectX::XMLoadFloat3(&center);
                DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Verts[i], Center);
                float dot = 0.0f;
                DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(Vec, Normal));
                if (dot <= 0.0f)return 0;
            }
            return -1;
        };

        //AABBの左側
        {
            DirectX::XMFLOAT3 center = { max.x,(max.y + min.y) * 0.5f,(max.z + min.z) * 0.5f };
            DirectX::XMFLOAT3 normal = { 1.0f,0.0f,0.0f };
            //視錐台との表裏判定
            if (Judge(normal, center, Verts) < 0)
            {
                return false;
            }

        }
        //AABBの右側
        {
            DirectX::XMFLOAT3 center = { min.x,(max.y + min.y) * 0.5f,(max.z + min.z) * 0.5f };
            DirectX::XMFLOAT3 normal = { -1.0f,0.0f,0.0f };
            //視錐台との表裏判定
            if (Judge(normal, center, Verts) < 0)
            {
                return false;
            }
        }
        //AABBの上側
        {
            DirectX::XMFLOAT3 center = { (max.x + min.x) * 0.5f, max.y,(max.z + min.z) * 0.5f };
            DirectX::XMFLOAT3 normal = { 0.0f,1.0f,0.0f };
            //視錐台との表裏判定
            if (Judge(normal, center, Verts) < 0)
            {
                return false;
            }
        }
        //AABBの下側
        {
            DirectX::XMFLOAT3 center = { (max.x + min.x) * 0.5f, min.y,(max.z + min.z) * 0.5f };
            DirectX::XMFLOAT3 normal = { 0.0f,-1.0f,0.0f };
            //視錐台との表裏判定
            if (Judge(normal, center, Verts) < 0)
            {
                return false;
            }
        }
        //AABBの手前側
        {
            DirectX::XMFLOAT3 center = { (max.x + min.x) * 0.5f, (max.y + min.y) * 0.5f, max.z };
            DirectX::XMFLOAT3 normal = { 0.0f,0.0f,1.0f };
            //視錐台との表裏判定
            if (Judge(normal, center, Verts) < 0)
            {
                return false;
            }
        }
        //AABBの奥側
        {
            DirectX::XMFLOAT3 center = { (max.x + min.x) * 0.5f, (max.y + min.y) * 0.5f, min.z };
            DirectX::XMFLOAT3 normal = { 0.0f,0.0f,-1.0f };
            //視錐台との表裏判定
            if (Judge(normal, center, Verts) < 0)
            {
                return false;
            }
        }
    }
    return true;
}

//-----< ベクトルと行列の乗算 >-----//
inline DirectX::XMFLOAT4 Float4MultiplyFloat4x4(DirectX::XMFLOAT4 a, DirectX::XMFLOAT4X4 b)
{
    float x = a.x * b._11 + a.y * b._12 + a.z * b._13 + a.w * b._14;
    float y = a.x * b._21 + a.y * b._22 + a.z * b._23 + a.w * b._24;
    float z = a.x * b._31 + a.y * b._32 + a.z * b._33 + a.w * b._34;
    float w = a.x * b._41 + a.y * b._42 + a.z * b._43 + a.w * b._44;

    return { x,y,z,w };
}

inline DirectX::XMFLOAT4 Float4x4MultiplyFloat4(DirectX::XMFLOAT4X4 a, DirectX::XMFLOAT4 b)
{
    float x = a._11 * b.x + a._21 * b.y + a._31 * b.z + a._41 * b.w;
    float y = a._12 * b.x + a._22 * b.y + a._32 * b.z + a._42 * b.w;
    float z = a._13 * b.x + a._23 * b.y + a._33 * b.z + a._43 * b.w;
    float w = a._14 * b.x + a._24 * b.y + a._34 * b.z + a._44 * b.w;

    return { x,y,z,w };
}

//-----< ワールド座標をスクリーン座標に変換----->
// worldPos : スクリーン座標に変換したいワールド座標
// cameraProjection : カメラのプロジェクション行列
// cameraView : カメラのビュー行列
inline DirectX::XMFLOAT2 GetScreenPosition(ID3D11DeviceContext* dc, const DirectX::XMFLOAT3 worldPos, const DirectX::XMFLOAT4X4 cameraProjection, const DirectX::XMFLOAT4X4 cameraView)
{
    // デバイスからスクリーンサイズ取得
    D3D11_VIEWPORT viewport;
    UINT num_viewports = 1;
    dc->RSGetViewports(&num_viewports, &viewport);

    //スクリーン座標に変換
    const DirectX::XMVECTOR Screen = DirectX::XMVector3Project(
        DirectX::XMLoadFloat3(&worldPos),//敵の位置
        viewport.TopLeftX, viewport.TopLeftY,//ビューポート左上の位置
        viewport.Width, viewport.Height,//ビューポートのサイズ
        viewport.MinDepth, viewport.MaxDepth,//深度値の範囲
        DirectX::XMLoadFloat4x4(&cameraProjection),//カメラのプロジェクション行列
        DirectX::XMLoadFloat4x4(&cameraView),//カメラのビュー行列
        DirectX::XMMatrixIdentity());//単位行列(ワールド行列);

    DirectX::XMFLOAT2 screen{};
    DirectX::XMStoreFloat2(&screen, Screen);

    return screen;
}

//-----< char型の文字列をstd::wstringに変換 >-----//
inline std::wstring ConvertCharToWchar(const char* cstr)
{
    const size_t length = strlen(cstr);

    wchar_t* wchar = static_cast<wchar_t*>(malloc(length * sizeof(wchar_t)));

    //システム規定のロケールに設定
    setlocale(LC_ALL, "");

    //マルチバイト文字をワイド文字に変換
    size_t size;
    mbstowcs_s(&size, wchar, length, cstr, length -1);

    std::wstring wstr = wchar;

    return wstr;
}

// stringをwstringへ変換する
static std::wstring StringToWString(std::string oString)
{
    // SJIS → wstring
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

    // バッファの取得
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS → wstring
    MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

    // stringの生成
    std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

    // バッファの破棄
    delete[] cpUCS2;

    // 変換結果を返す
    return(oRet);
}

namespace Random
{
    // minとmaxの間のランダムの値を返す
    static float Range(float min, float max)
    {
        float value = static_cast<float>(rand()) / RAND_MAX;
        return min + (max - min) * value;
    }
}