#include "Light.hlsli"

//-----< �萔�o�b�t�@�ꗗ >-----//


cbuffer CAMERA : register(b0)
{
	//�J����
	row_major float4x4 viewProjection;
	float4 cameraPosition;
}

//cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
//{
//	//�I�u�W�F�N�g�p
//};

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

//struct InstancingMatrix
//{
//	float4 position;//w��
//	float4 scale;//w��
//	float4 texcoord;//z,w��
//	float4 color;
//};
//cbuffer SpriteInstancingBuffer : register(b9)
//{
//	//�C���X�^���V���O
//	InstancingMatrix instancingMatrix[1024];
//}






//-----< �e�N�X�`�� >-----//
#define BaseColor 0
#define NormalMap 1
#define AO 2
#define Emissive 3
#define Metallic 4
#define Roughness 5