
// 点光源
struct PointLight
{
	float4	pos;
	float4	color;
	float	range;
	float3	dummy;
};

// スポットライト
struct SpotLight
{
	float4	pos;
	float4	direction;
	float4	color;
	float	range;
	float	innerCorn;
	float	outerCorn;
	float	dummy;
};
