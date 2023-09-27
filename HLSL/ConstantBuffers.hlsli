#include "Light.hlsli"

//-----< 定数バッファ一覧 >-----//


cbuffer CAMERA : register(b0)
{
	//カメラ
	row_major float4x4 viewProjection;
	float4 cameraPosition;
}

//cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
//{
//	//オブジェクト用
//};

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

//struct InstancingMatrix
//{
//	float4 position;//w空き
//	float4 scale;//w空き
//	float4 texcoord;//z,w空き
//	float4 color;
//};
//cbuffer SpriteInstancingBuffer : register(b9)
//{
//	//インスタンシング
//	InstancingMatrix instancingMatrix[1024];
//}






//-----< テクスチャ >-----//
#define BaseColor 0
#define NormalMap 1
#define AO 2
#define Emissive 3
#define Metallic 4
#define Roughness 5