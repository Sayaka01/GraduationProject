struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
};

cbuffer CAMERA : register(b0)
{
	//ÉJÉÅÉâ
	row_major float4x4 viewProjection;
	float4 cameraPosition;
}

cbuffer OBJECT_CONSTANT_BUFFER : register(b1)
{
	row_major float4x4 world;
	float4 materialColor;
};

