
// �_����
struct PointLight
{
	float4	pos;
	float4	color;
	float	range;
	float3	dummy;
};

// �X�|�b�g���C�g
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
