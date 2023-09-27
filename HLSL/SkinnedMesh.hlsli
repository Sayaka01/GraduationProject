
#include "Light.hlsli"

struct VS_IN
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texcoord : TEXCOORD;
	float4 boneWeights : WEIGHTS;
	uint4 boneIndices : BONES;
};
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 worldNormal : NORMAL;
	float4 worldTangent : TANGENT;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
	float3 shadowTexcoord : SHADOW_TEXCOORD;
};

static const int MAX_BONES = 256;

cbuffer CAMERA : register(b0)
{
	//カメラ
	row_major float4x4 viewProjection;
	float4 cameraPosition;
}

cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
{
	row_major float4x4 world;
	float4 materialColor;
	row_major float4x4 boneTransforms[MAX_BONES];
};

cbuffer DIRECTION_LIGHT : register(b2)
{
	//ディレクションライト
	float4 ambientColor;
	float4 lightDirection;
	float4 lightColor;
}

cbuffer POINT_LIGHT : register(b3)
{
	//ポイントライト
	PointLight pointLights[8];
}

cbuffer SPOT_LIGHT : register(b4)
{
	//スポットライト
	SpotLight spotLights[8];
}

cbuffer SHADOWMAP : register(b5)
{
	row_major float4x4 lightViewProjection; //ライトの位置から見た射影行列
	float3 shadowColor; //影色
	float shadowBias;	//深度バイアス
}

cbuffer FOG : register(b6)
{
	//フォグ
	float4 fogColor;
	float4 fogRange;
}

cbuffer PBR : register(b7)
{
	float metallic;
	float smooth;
	float occlusionStrength;
	float emissiveIntensity;
	float pureWhite;//初期値:3.0f
	float3 dummy;
	float4 emissiveColor;
}