
struct VS_IN
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};