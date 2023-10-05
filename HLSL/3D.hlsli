
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
	//�J����
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
	//�f�B���N�V�������C�g
	float4 ambientColor;
	float4 lightDirection;
	float4 lightColor;
}

cbuffer POINT_LIGHT : register(b3)
{
	//�|�C���g���C�g
	PointLight pointLights[8];
}

cbuffer SPOT_LIGHT : register(b4)
{
	//�X�|�b�g���C�g
	SpotLight spotLights[8];
}

cbuffer SHADOWMAP : register(b5)
{
	row_major float4x4 lightViewProjection; //���C�g�̈ʒu���猩���ˉe�s��
	float3 shadowColor; //�e�F
	float shadowBias;	//�[�x�o�C�A�X
}

cbuffer FOG : register(b6)
{
	//�t�H�O
	float4 fogColor;
	float4 fogRange;
}

cbuffer PBR : register(b7)
{
	float metallic;
	float smooth;
	float occlusionStrength;
	float emissiveIntensity;
	float pureWhite;//�����l:3.0f
	float3 dummy;
	float4 emissiveColor;
}